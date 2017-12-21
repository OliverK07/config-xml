#include "SaxParser.h"
#include "SaxParserHandler.h"
#include "SaxParserException.h"
#include "Path.h"

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/framework/XMLPScanToken.hpp>
#include <xercesc/sax/Locator.hpp>

// Remove to re-enable debugging

#undef CONN_LOG_DEBUG
#define CONN_LOG_DEBUG(context,msg)

namespace conn
{

class XercesInitializer
{
public:
   XercesInitializer ()
   {
      xercesc::XMLPlatformUtils::Initialize ();
   }

   ~XercesInitializer ()
   {
      xercesc::XMLPlatformUtils::Terminate ();
   }
};
static XercesInitializer init;

static void throwSaxParserException (Logger *logger, Logger::Level level, const xercesc::SAXParseException &exc)
{
   char *tmp = xercesc::XMLString::transcode (exc.getSystemId ());
   std::string systemid (tmp);
   xercesc::XMLString::release (&tmp);

   tmp = xercesc::XMLString::transcode (exc.getMessage ());
   std::string message (tmp);
   xercesc::XMLString::release (&tmp);

   SaxParserException e (systemid, exc.getLineNumber (), exc.getColumnNumber (), message);
   CONN_LOG_MSG (logger, level, e.what ());
   throw e;
}

class SaxParserImpl : private xercesc::ErrorHandler, private xercesc::DocumentHandler
{
public:
   SaxParserImpl (Logger *logger, SaxParser::Validation validation);
   ~SaxParserImpl ();

   void loadGrammar (const Path &file);
   void unloadGrammar (const Path &file);
   void parse (const Path &file, SaxParserHandler &handler);

private: // xercesc::ErrorHandler
   void warning (const xercesc::SAXParseException &exc);
   void error (const xercesc::SAXParseException &exc);
   void fatalError (const xercesc::SAXParseException &exc);
   void resetErrors ();

private: // xercesc::DocumentHandler
   void resetDocument ();
   void setDocumentLocator (const xercesc::Locator * const locator);
   void startDocument ();
   void startElement (const XMLCh * const xname, xercesc::AttributeList &xattrs);
   void ignorableWhitespace (const XMLCh * const, const XMLSize_t);
   void characters (const XMLCh * const, const XMLSize_t);
   void endElement (const XMLCh * const);
   void endDocument ();
   void processingInstruction (const XMLCh * const, const XMLCh * const);

private:
   std::string getSystemId ();
   XMLFileLoc getLineNumber ();
   XMLFileLoc getColumnNumber ();

private:
   Logger                 *m_logger;
   xercesc::SAXParser      m_parser;
   const xercesc::Locator *m_locator;
   SaxParserHandler       *m_handler;
   std::string             m_data;
};

SaxParserImpl::SaxParserImpl (Logger *logger, SaxParser::Validation validation)
   : m_logger (logger), m_locator (0), m_handler (0)
{
   m_parser.setErrorHandler (this);
   m_parser.setDocumentHandler (this);
   m_parser.setValidationSchemaFullChecking (true);
   m_parser.useCachedGrammarInParse (true);
   m_parser.setDoNamespaces (true);
   m_parser.setDoSchema (true);
   m_parser.setExitOnFirstFatalError (true);
   m_parser.setValidationConstraintFatal (true);

   switch (validation)
   {
      case SaxParser::NEVER:
      {
         m_parser.setValidationScheme (m_parser.Val_Never);
         break;
      }
      case SaxParser::AUTO:
      {
         m_parser.setValidationScheme (m_parser.Val_Auto);
         break;
      }
      case SaxParser::ALWAYS:
      {
         m_parser.setValidationScheme (m_parser.Val_Always);
         break;
      }
   }
}

SaxParserImpl::~SaxParserImpl ()
{
}

void SaxParserImpl::loadGrammar (const Path &file)
{
   xercesc::Grammar *grammar = m_parser.loadGrammar (file.c_str (), xercesc::Grammar::SchemaGrammarType, true);

   char *tns = xercesc::XMLString::transcode (grammar->getTargetNamespace());
   CONN_LOG_INFO (m_logger, "Loaded grammar file " << file << " (targetNamepace=\"" << tns << "\")");
   xercesc::XMLString::release (&tns);
}

void SaxParserImpl::unloadGrammar (const Path &file)
{
   CONN_LOG_INFO (m_logger, "Unload grammar file " << file << " not supported yet");
   //m_parser.getGrammar();
   //m_parser.getGrammarResolver();
   //m_parser.resetCachedGrammarPool();
}

void SaxParserImpl::parse (const Path &file, SaxParserHandler &handler)
{
   m_handler = &handler;

   try
   {
      m_parser.parse (file.c_str ());
      m_handler = 0;
   }
   catch (...)
   {
      m_handler = 0;
      throw;
   }
}

void SaxParserImpl::warning (const xercesc::SAXParseException& exc)
{
   throwSaxParserException (m_logger, Logger::CONN_LOGGER_WARN, exc);
}

void SaxParserImpl::error (const xercesc::SAXParseException& exc)
{
   throwSaxParserException (m_logger, Logger::CONN_LOGGER_ERROR, exc);
}

void SaxParserImpl::fatalError (const xercesc::SAXParseException& exc)
{
   throwSaxParserException (m_logger, Logger::CONN_LOGGER_ERROR, exc);
}

void SaxParserImpl::resetErrors ()
{
}

void SaxParserImpl::resetDocument ()
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);
   m_locator = 0;
}

void SaxParserImpl::setDocumentLocator (const xercesc::Locator * const locator)
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);
   m_locator = locator;
}

void SaxParserImpl::startDocument ()
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);
}

void SaxParserImpl::startElement(const XMLCh * const xname, xercesc::AttributeList &xattrs)
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);

   // Fill the collection of attribute key-value pairs.
   SaxParserHandler::AttributeMap attributes;
   {
      for (XMLSize_t i = 0; i < xattrs.getLength (); i++)
      {
         char * att_name = xercesc::XMLString::transcode (xattrs.getName (i));
         char * att_value = xercesc::XMLString::transcode (xattrs.getValue (i));

         attributes[att_name] = att_value;

         xercesc::XMLString::release(&att_name);
         xercesc::XMLString::release(&att_value);
      }
   };

   char *name = xercesc::XMLString::transcode (xname);

   assert (m_handler);
   m_handler->onElementStart (name, attributes, getSystemId (), getLineNumber());
   xercesc::XMLString::release (&name);

   m_data = "";
}

void SaxParserImpl::ignorableWhitespace(const XMLCh * const, const XMLSize_t)
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);
}

void SaxParserImpl::characters (const XMLCh * const xchars, const XMLSize_t length)
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);

   char * chars = xercesc::XMLString::transcode (xchars);
   std::string characterData (chars, length);
   xercesc::XMLString::release(&chars);

   m_data += characterData;
}

void SaxParserImpl::endElement (const XMLCh * const xname)
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);

   char * name = xercesc::XMLString::transcode (xname);

   assert (m_handler);
   m_handler->onElementEnd (name, m_data);
   xercesc::XMLString::release(&name);

   m_data = "";
}

void SaxParserImpl::endDocument()
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);
}

void SaxParserImpl::processingInstruction(const XMLCh*, const XMLCh*)
{
   CONN_LOG_DEBUG (m_logger, "DocumentHandler::" << __func__);
}

std::string SaxParserImpl::getSystemId ()
{
   assert (m_locator);
   char *tmp = xercesc::XMLString::transcode (m_locator->getSystemId ());
   std::string result (tmp);
   xercesc::XMLString::release (&tmp);
   return result;
}

XMLFileLoc SaxParserImpl::getLineNumber ()
{
   assert (m_locator);
   return m_locator->getLineNumber ();
}

XMLFileLoc SaxParserImpl::getColumnNumber ()
{
   assert (m_locator);
   return m_locator->getColumnNumber ();
}

SaxParser::SaxParser (Logger *logger, Validation validation) :
   m_impl (new SaxParserImpl (logger, validation))
{
}

SaxParser::~SaxParser ()
{
   delete m_impl;
   m_impl = 0;
}

void SaxParser::loadGrammar (const Path &file)
{
   assert (m_impl);
   m_impl->loadGrammar (file);
}

void SaxParser::unloadGrammar (const Path &file)
{
   assert (m_impl);
   m_impl->unloadGrammar (file);
}

void SaxParser::parse (const Path &file, SaxParserHandler &handler)
{
   assert (m_impl);
   m_impl->parse (file, handler);
}

}
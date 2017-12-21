#ifndef _CONN_SAXPARSER_H_
#define _CONN_SAXPARSER_H_

#include "Conn.h"
#include "Logger.h"
#include "SaxParserHandler.h"
#include "SaxParserException.h"

namespace conn
{

class SaxParserImpl;

/**
 * SAX parser. Parses an XML file invoking callbacks on associated handler.
 */
class CONN_EXPORT SaxParser
{
public:
   enum Validation { NEVER, AUTO, ALWAYS };

   SaxParser (Logger *logger = Logger::root (), Validation validation = ALWAYS);
   ~SaxParser ();

   void loadGrammar (const Path &file);
   void unloadGrammar (const Path &file);

   void parse (const Path &file, SaxParserHandler &handler);

private:
   SaxParser (const SaxParser &);               // not implemented
   SaxParser & operator = (const SaxParser &);  // not implemented

   SaxParserImpl *m_impl;
};

}

#endif
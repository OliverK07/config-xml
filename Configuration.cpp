#include <cstdio>
#include <typeinfo>
#include "Configuration.h"
#include "SaxParserHandler.h"
#include "SaxParser.h"
#include "Path.h"

namespace conn
{

const Configuration &Configuration::NONE (std::string(""));

class XmlConfigurationHandler : public SaxParserHandler
{
public:

   XmlConfigurationHandler () : m_root (0) {}
   virtual ~XmlConfigurationHandler () {}

   virtual void onElementStart (const std::string &element, const AttributeMap &attributes, const std::string &file, uint64_t line)
   {
      Configuration *node = 0;

      if (m_root == 0)
      {
         assert (m_stack.empty ());
         m_root = new Configuration (element, file, line);
         node = m_root.ptr ();
      }
      else
      {
         node = m_stack.top()->createChild (element, file, line);
      }

      m_stack.push (node);

      for (AttributeMap::const_iterator i = attributes.begin (); i != attributes.end (); i++)
      {
         node->setAttribute (i->first, i->second);
      }
   }

   virtual void onElementEnd (const std::string & element, const std::string & data)
   {
      assert (!m_stack.empty ());
      // FIXME: No way of telling if the element had content or not
      m_stack.top()->setValue (data);
      m_stack.pop ();
   }

   HConfiguration m_root;
   std::stack<Configuration*> m_stack;
};

HConfiguration Configuration::load (SaxParser &parser, const Path &file)
{
   XmlConfigurationHandler handler;
   parser.parse (file, handler);
   return handler.m_root;
}

Configuration::Configuration (const std::string &name)
   : m_parent (0), m_name (name), m_line (0)
{
}

Configuration::Configuration (const std::string &name, const std::string &file, uint64_t line)
   : m_parent (0), m_name (name), m_file (file), m_line (line)
{
}

Configuration::Configuration (const Configuration *parent, const std::string &name, const std::string &file, uint64_t line)
   : m_parent (parent), m_name (name), m_file (file), m_line (line)
{
}

Configuration::~Configuration ()
{
   for (ChildrenMap::iterator i = m_children.begin (); i != m_children.end (); i++)
   {
      i->second->m_parent = 0;
   }
}

bool Configuration::empty () const
{
   return m_attributes.empty () && m_children.empty ();
}

const std::string & Configuration::getName () const
{
   return m_name;
}

std::string Configuration::getFullName () const
{
   if (m_parent == 0)
   {
      return m_name;
   }
   else
   {
      return m_parent->getFullName () + "/" + m_name;
   }
}

std::string Configuration::getLocation () const
{
   static const std::string def ("<unknown location>");

   if (m_file.empty ())
   {
      return def;
   }
   else
   {
      std::ostringstream ss;
      ss << m_file << ':' << m_line;
      return ss.str ();
   }
}

bool Configuration::hasChild (const std::string &name) const
{
   return (m_children.find (name) != m_children.end());
}

const Configuration * Configuration::getChild (const std::string &name) const
{
   ChildrenMap::const_iterator it = m_children.find (name);
   if (it != m_children.end())
   {
      return it->second.ptr ();
   }
   else
   {
      throw ConfigurationException (this, "No such child <" + name + "> in <" + getFullName () + ">");
   }
}

Configuration::Children Configuration::getChildren (const std::string &name) const
{
   Children result;
   std::pair<ChildrenMap::const_iterator,ChildrenMap::const_iterator> found = m_children.equal_range (name);

   for (ChildrenMap::const_iterator i = found.first; i != found.second; i++)
   {
      result.push_back (i->second.ptr ());
   }

   return result;
}

Configuration::Children Configuration::getChildren () const
{
   Children result;

   for (ChildrenMap::const_iterator i = m_children.begin (); i != m_children.end (); i++)
   {
      result.push_back (i->second.ptr ());
   }

   return result;
}

std::string Configuration::getAttributeAsString (const std::string &name) const
{
   AttributeMap::const_iterator it = m_attributes.find (name);

   if (it != m_attributes.end())
   {
      return it->second;
   }
   else
   {
      throw ConfigurationException (this, "No such attribute : \"" + name + "\" in <" + getFullName () + ">");
   }
}

int32_t Configuration::getAttributeAsInteger (const std::string &name) const
{
   return to_int32_t (name, getAttributeAsString (name));
}

uint32_t Configuration::getAttributeAsUInteger (const std::string &name) const
{
   return to_uint32_t (name, getAttributeAsString (name));
}

int64_t Configuration::getAttributeAsLong (const std::string &name) const
{
   return to_int64_t (name, getAttributeAsString (name));
}

uint64_t Configuration::getAttributeAsULong (const std::string &name) const
{
   return to_uint64_t (name, getAttributeAsString (name));
}

bool Configuration::getAttributeAsBoolean (const std::string &name) const
{
   return to_bool (name, getAttributeAsString (name));
}

float Configuration::getAttributeAsFloat (const std::string &name) const
{
   return to_float (name, getAttributeAsString (name));
}

std::string Configuration::getAttributeAsString (const std::string &name, const std::string &dflt) const
{
   AttributeMap::const_iterator i = m_attributes.find (name);
   return (i == m_attributes.end()) ? dflt : i->second;
}

int32_t Configuration::getAttributeAsInteger (const std::string &name, int32_t dflt) const
{
   AttributeMap::const_iterator i = m_attributes.find (name);
   return (i == m_attributes.end()) ? dflt : to_int32_t (name, i->second);
}

uint32_t Configuration::getAttributeAsUInteger (const std::string &name, uint32_t dflt) const
{
   AttributeMap::const_iterator i = m_attributes.find (name);
   return (i == m_attributes.end()) ? dflt : to_uint32_t (name, i->second);
}

int64_t Configuration::getAttributeAsLong (const std::string &name, int64_t dflt) const
{
   AttributeMap::const_iterator i = m_attributes.find (name);
   return (i == m_attributes.end()) ? dflt : to_int64_t (name, i->second);
}

uint64_t Configuration::getAttributeAsULong (const std::string &name, uint64_t dflt) const
{
   AttributeMap::const_iterator i = m_attributes.find (name);
   return (i == m_attributes.end()) ? dflt : to_uint64_t (name, i->second);
}

bool Configuration::getAttributeAsBoolean (const std::string &name, bool dflt) const
{
   AttributeMap::const_iterator i = m_attributes.find (name);
   return (i == m_attributes.end()) ? dflt : to_bool (name, i->second);
}

float Configuration::getAttributeAsFloat (const std::string &name, float dflt) const
{
   AttributeMap::const_iterator i = m_attributes.find (name);
   return (i == m_attributes.end()) ? dflt : to_float (name, i->second);
}

const std::string &Configuration::getValueAsString () const
{
   if (m_children.size ())
   {
      throw ConfigurationException (this, "Element <" + m_name + "> has got no value. It has children.");
   }

   return m_value;
}

int32_t Configuration::getValueAsInteger () const
{
   return to_int32_t ("", getNonBlankValueAsString ());
}

uint32_t Configuration::getValueAsUInteger () const
{
   return to_uint32_t ("", getNonBlankValueAsString ());
}

int64_t Configuration::getValueAsLong () const
{
   return to_int64_t ("", getNonBlankValueAsString ());
}

uint64_t Configuration::getValueAsULong () const
{
   return to_uint64_t ("", getNonBlankValueAsString ());
}

bool Configuration::getValueAsBoolean () const
{
   return to_bool ("", getNonBlankValueAsString ());
}

float Configuration::getValueAsFloat () const
{
   return to_float ("", getNonBlankValueAsString ());
}


const std::string &Configuration::getValueAsString (const std::string &dflt) const
{
   return m_value.empty () ? dflt : getNonBlankValueAsString ();
}

int32_t Configuration::getValueAsInteger (int32_t dflt) const
{
   return m_value.empty () ? dflt : getValueAsInteger ();
}

uint32_t Configuration::getValueAsUInteger (uint32_t dflt) const
{
   return m_value.empty () ? dflt : getValueAsUInteger ();
}

int64_t Configuration::getValueAsLong (int64_t dflt) const
{
   return m_value.empty () ? dflt : getValueAsLong ();
}

uint64_t Configuration::getValueAsULong (uint64_t dflt) const
{
   return m_value.empty () ? dflt : getValueAsULong ();
}

bool Configuration::getValueAsBoolean (bool dflt) const
{
   return m_value.empty () ? dflt : getValueAsBoolean ();
}

float Configuration::getValueAsFloat (float dflt) const
{
   return m_value.empty () ? dflt : getValueAsFloat ();
}

const std::string & Configuration::getChildValueAsString (const std::string &name) const
{
   return getChild(name)->m_value;
}

int32_t Configuration::getChildValueAsInteger (const std::string &name) const
{
   return getChild(name)->getValueAsInteger ();
}

uint32_t Configuration::getChildValueAsUInteger (const std::string &name) const
{
   return getChild(name)->getValueAsUInteger ();
}

int64_t Configuration::getChildValueAsLong (const std::string &name) const
{
   return getChild(name)->getValueAsLong ();
}

uint64_t Configuration::getChildValueAsULong (const std::string &name) const
{
   return getChild(name)->getValueAsULong ();
}

bool Configuration::getChildValueAsBoolean (const std::string &name) const
{
   return getChild(name)->getValueAsBoolean ();
}

float Configuration::getChildValueAsFloat (const std::string &name) const
{
   return getChild(name)->getValueAsFloat ();
}

const std::string & Configuration::getChildValueAsString (const std::string &name, const std::string &dflt) const
{
   ChildrenMap::const_iterator i (m_children.find (name));
   return (i == m_children.end ()) ? dflt : i->second->m_value;
}

int32_t Configuration::getChildValueAsInteger (const std::string &name, int32_t dflt) const
{
   ChildrenMap::const_iterator i (m_children.find (name));
   return (i == m_children.end ()) ? dflt : i->second->getValueAsInteger ();
}

uint32_t Configuration::getChildValueAsUInteger (const std::string &name, uint32_t dflt) const
{
   ChildrenMap::const_iterator i (m_children.find (name));
   return (i == m_children.end ()) ? dflt : i->second->getValueAsUInteger ();
}

int64_t Configuration::getChildValueAsLong (const std::string &name, int64_t dflt) const
{
   ChildrenMap::const_iterator i (m_children.find (name));
   return (i == m_children.end ()) ? dflt : i->second->getValueAsLong ();
}

uint64_t Configuration::getChildValueAsULong (const std::string &name, uint64_t dflt) const
{
   ChildrenMap::const_iterator i (m_children.find (name));
   return (i == m_children.end ()) ? dflt : i->second->getValueAsULong ();
}

bool Configuration::getChildValueAsBoolean (const std::string &name, bool dflt) const
{
   ChildrenMap::const_iterator i (m_children.find (name));
   return (i == m_children.end ()) ? dflt : i->second->getValueAsBoolean ();
}

float Configuration::getChildValueAsFloat (const std::string &name, float dflt) const
{
   ChildrenMap::const_iterator i (m_children.find (name));
   return (i == m_children.end ()) ? dflt : i->second->getValueAsFloat ();
}


void Configuration::setAttribute (const std::string &name, const std::string &value)
{
   m_attributes.insert (std::pair<std::string,std::string>(name, value));
}

void Configuration::setValue (const std::string &value)
{
   assert (m_value.empty ());
   m_value = value;
}

Configuration *Configuration::createChild (const std::string &name)
{
   return createChild (name, m_file, m_line);
}

Configuration *Configuration::createChild (const std::string &name, const std::string &file, uint64_t line)
{
   Configuration *child = new Configuration (this, name, file, line);

   m_children.insert (ChildrenMap::value_type (name, HConfiguration (child)));

   return child;
}

const std::string &Configuration::getNonBlankValueAsString () const
{
   if (m_value.empty ())
   {
      throw ConfigurationException (this, "Element <" + m_name + "> has got no value");
   }

   return m_value;
}

bool Configuration::to_bool (const std::string &attribute, const std::string &str) const
{
   bool result = false;

   if ((str == "true") || (str == "True") || (str == "TRUE") || (str == "1"))
   {
      result = true;
   }
   else if ((str != "false") && (str != "False") && (str != "FALSE") && (str != "0"))
   {
      throwFailedToConvert (attribute, str, "bool");
   }

   return result;
}

int32_t Configuration::to_int32_t (const std::string &attribute, const std::string &str) const
{
   int32_t result;

   if (sscanf (str.c_str (), "%d", &result) != 1)
   {
      throwFailedToConvert (attribute, str, "int32_t");
   }

   return result;
}

uint32_t Configuration::to_uint32_t (const std::string &attribute, const std::string &str) const
{
   uint32_t result;

   if (sscanf (str.c_str (), "%u", &result) != 1)
   {
      throwFailedToConvert (attribute, str, "uint32_t");
   }

   return result;
}

int64_t Configuration::to_int64_t (const std::string &attribute, const std::string &str) const
{
   int64_t result;

#ifdef PRId64
   if (sscanf (str.c_str (), "%" PRId64, &result) != 1)
#else
   if (sscanf (str.c_str (), "%ld", &result) != 1)
#endif
   {
      throwFailedToConvert (attribute, str, "int64_t");
   }

   return result;
}

uint64_t Configuration::to_uint64_t (const std::string &attribute, const std::string &str) const
{
   uint64_t result;

#ifdef PRIu64
   if (sscanf (str.c_str (), "%" PRIu64, &result) != 1)
#else
   if (sscanf (str.c_str (), "%lu", &result) != 1)
#endif
   {
      throwFailedToConvert (attribute, str, "uint64_t");
   }

   return result;
}

float Configuration::to_float (const std::string &attribute, const std::string &str) const
{
   float result;

   if (sscanf (str.c_str (), "%f", &result) != 1)
   {
      throwFailedToConvert (attribute, str, "float");
   }

   return result;
}

void Configuration::throwConfigurationException (const std::string &message) const
{
   throw ConfigurationException (this, message);
}

void Configuration::throwFailedToConvert (const std::string &attribute, const std::string &value, const std::string &type) const
{
   throw ConfigurationException (this, "Failed to convert \"" + value + "\" to " + type + " for " + (attribute.empty () ? "" : "attribute '" + attribute + "' in ") + "element <" + m_name + ">");
}

ConfigurationException::ConfigurationException
   (const Configuration *node, const std::string & message) :
   m_configuration (node),
   m_message (message)
{
}

const char *ConfigurationException::what() const throw()
{
   if (m_what.empty())
   {
      m_what = m_configuration->getLocation() + " : " + m_message;
   }

   return m_what.c_str();
}

}
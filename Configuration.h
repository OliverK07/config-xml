#ifndef _CONN_CONFIGURATION_H_
#define _CONN_CONFIGURATION_H_

#include "Conn.h"
#include "Counted.h"
#include "Handle.h"

namespace conn
{

/**
 * The Configuration object is based on a subset of XML's single-rooted hierarchical
 * tree structure where each Configuration node can contain multiple attributes, and
 * leaf nodes can also contain a value. Reflecting this, Configuration objects can
 * be built from an XML file.
 * The Container (just handwritten main() code for now) will instantiate the
 * appropriate Configuration object(s) and then pass them into the
 * Lifecycle::configure() method on each object that supports the Lifecycle
 * interface.
 */
class CONN_EXPORT Configuration : public CountedBase
{
public:

   /**
    * Loads an XML file into a Configuration object.
    *
    * @return A new Configuration object that represents the content of the
    * loaded XML file. Ownership of the Configuration is returned to the caller.
    */
   static HConfiguration load (SaxParser &parser, const Path &file);

public:

   typedef std::vector<Configuration*> Children;

   /**
    * Construct a root Configuration node with the specified name and unspecified location
    */
   Configuration (const std::string &name);

   /**
    * Construct a root Configuration node with the specified name and location
    */
   Configuration (const std::string &name, const std::string &file, uint64_t line);

   /**
    *
    */
   virtual ~Configuration ();

   /**
    * An empty root Configuration singleton object
    */
   static const Configuration &NONE;

   /**
    * Get the name of this Configuration node
    */
   const std::string & getName () const;

   /**
    * Get the full scoped name including all parents
    */
   std::string getFullName () const;

   /**
    * Get the location of where this Configuration node came from. This
    * can be used by clients when reporting errors with configuration data.
    */
   std::string getLocation () const;

   /**
    * Returns true if this Configuration node has one or more child nodes with the specified name
    */
   bool hasChild (const std::string &name) const;

   /**
    * Get the first child Configuration node with the specified name
    */
   const Configuration *getChild (const std::string &name) const;

   /**
    * Get all the direct children of this node with the specified name.
    */
   Children getChildren (const std::string &name) const;

   /**
    * Get all the direct children of this node.
    */
   Children getChildren () const;

   /**
    * Return a string value with the specified name. If this node does not
    * have a string value with that name, and exception will be thrown
    */
   std::string getAttributeAsString (const std::string &name) const;

   /**
    * Return a signed 32 bit integer value with the specified name. If this node does not
    * have a signed 32 bit integer value with that name, and exception will be thrown
    */
   int32_t getAttributeAsInteger (const std::string &name) const;

   /**
    * Return an unsigned 32 bit integer value with the specified name. If this node does not
    * have an unsigned 32 bit integer value with that name, and exception will be thrown
    */
   uint32_t getAttributeAsUInteger (const std::string &name) const;

   /**
    * Return a signed 64 bit integer with the specified name. If this node does not
    * have a signed 64 bit integer value with that name, and exception will be thrown
    */
   int64_t getAttributeAsLong (const std::string &name) const;

   /**
    * Return an unsigned 64 bit integer with the specified name. If this node does not
    * have an unsigned 64 bit integer value with that name, and exception will be thrown
    */
   uint64_t getAttributeAsULong (const std::string &name) const;

   /**
    * Return a boolean value with the specified name. If this node does not
    * have a boolean value with that name, and exception will be thrown
    */
   bool getAttributeAsBoolean (const std::string &name) const;

   /**
    * Return a float value with the specified name. If this node does not
    * have a float value with that name, and exception will be thrown
    */
   float getAttributeAsFloat (const std::string &name) const;

   /**
    * Return a string value with the specified name. If this node does not
    * have a string value with that name, return the specified default value
    */
   std::string getAttributeAsString (const std::string &name, const std::string &dflt) const;

   /**
    * Return a signed  integer value with the specified name. If this node does not
    * have a signed integer value with that name, return the specified default value
    */
   int32_t getAttributeAsInteger (const std::string &name, int32_t dflt) const;

   /**
    * Return an unsigned integer value with the specified name. If this node does not
    * have an unsigned integer value with that name, return the specified default value
    */
   uint32_t getAttributeAsUInteger (const std::string &name, uint32_t dflt) const;

   /**
    * Return a 64 bit integer with the specified name. If this node does not
    * have a 64 bit integer with that name, return the specified default value
    */
   int64_t getAttributeAsLong (const std::string &name, int64_t dflt) const;

   /**
    * Return an unsigned 64 bit integer with the specified name. If this node does not
    * have an unsigned 64 bit integer with that name, return the specified default value
    */
   uint64_t getAttributeAsULong (const std::string &name, uint64_t dflt) const;

   /**
    * Return a boolean value with the specified name. If this node does not
    * have a boolean value with that name, return the specified default value
    */
   bool getAttributeAsBoolean (const std::string &name, bool dflt) const;

   /**
    * Return a float value with the specified name. If this node does not
    * have a float value with that name, return the specified default value
    */
   float getAttributeAsFloat (const std::string &name, float dflt) const;

   const std::string & getValueAsString () const;
   int32_t getValueAsInteger () const;
   uint32_t getValueAsUInteger () const;
   int64_t getValueAsLong () const;
   uint64_t getValueAsULong () const;
   bool getValueAsBoolean () const;
   float getValueAsFloat () const;

   const std::string & getValueAsString (const std::string &dflt) const;
   int32_t getValueAsInteger (int32_t dflt) const;
   uint32_t getValueAsUInteger (uint32_t dflt) const;
   int64_t getValueAsLong (int64_t dflt) const;
   uint64_t getValueAsULong (uint64_t dflt) const;
   bool getValueAsBoolean (bool dflt) const;
   float getValueAsFloat (float dflt) const;

   const std::string & getChildValueAsString (const std::string &name) const;
   int32_t getChildValueAsInteger (const std::string &name) const;
   uint32_t getChildValueAsUInteger (const std::string &name) const;
   int64_t getChildValueAsLong (const std::string &name) const;
   uint64_t getChildValueAsULong (const std::string &name) const;
   bool getChildValueAsBoolean (const std::string &name) const;
   float getChildValueAsFloat (const std::string &name) const;

   const std::string & getChildValueAsString (const std::string &name, const std::string &dflt) const;
   int32_t getChildValueAsInteger (const std::string &name, int32_t dflt) const;
   uint32_t getChildValueAsUInteger (const std::string &name, uint32_t dflt) const;
   int64_t getChildValueAsLong (const std::string &name, int64_t dflt) const;
   uint64_t getChildValueAsULong (const std::string &name, uint64_t dflt) const;
   bool getChildValueAsBoolean (const std::string &name, bool dflt) const;
   float getChildValueAsFloat (const std::string &name, float dflt) const;

public: // Creating and setting stuff

   /**
    * Set a string value on this node. If this node already has a value with
    * the specified name, it will be overwritten.
    */
   void setAttribute (const std::string &name, const std::string &value);

   template<class T> void setAttribute (const std::string &name, const T &value)
   {
      std::ostringstream s;
      s << value;
      setAttribute (name, s.str ());
   }

   /**
    * Sets a value on the node itself (like XML CDATA)
    */
   void setValue (const std::string &value);

   /**
    * Create a new child node with the specified name, and an unspecified location
    *
    * \param name The name of the new child node
    *
    * \return the new child node
    */
   Configuration *createChild (const std::string &name);

   /**
    * Create a new child node with the specified name and location
    *
    * \param name The name of the new child node
    * \param location Where the new child node came from e.g. "MyConfig.xml:47"
    *
    * \return the new child node
    */
   Configuration *createChild (const std::string &name, const std::string &file, uint64_t line);

   /**
    * Returns if configuration is empty (contains no children or attributes).
    */
   bool empty () const;


   void throwConfigurationException (const std::string &message) const;

private:

   friend class CONN_EXPORT ConfigurationUtest;

   Configuration (const Configuration *parent, const std::string &name, const std::string &file, uint64_t line);

   const std::string & getNonBlankValueAsString () const;

   bool to_bool (const std::string &attribute, const std::string &str) const;
   int32_t to_int32_t (const std::string &attribute, const std::string &str) const;
   uint32_t to_uint32_t (const std::string &attribute, const std::string &str) const;
   int64_t to_int64_t (const std::string &attribute, const std::string &str) const;
   uint64_t to_uint64_t (const std::string &attribute, const std::string &str) const;
   float to_float (const std::string &attribute, const std::string &str) const;

   void throwFailedToConvert (const std::string &attribute, const std::string &value, const std::string &type) const;

private:
   typedef std::map<std::string,std::string> AttributeMap;
   typedef std::multimap<std::string,HConfiguration> ChildrenMap;

   const Configuration *m_parent;
   const std::string    m_name;
   const std::string    m_file;
   const uint64_t       m_line;
   std::string  m_value;
   AttributeMap m_attributes;
   ChildrenMap  m_children;
};

class CONN_EXPORT ConfigurationException : public std::exception
{
public:
   ConfigurationException (const Configuration *node, const std::string & message);
   virtual ~ConfigurationException () throw() {}
   const char * what () const throw();

private:
   const Handle<const Configuration> m_configuration;
   const std::string m_message;
   mutable std::string m_what;
};

}

#endif
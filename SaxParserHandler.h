#ifndef _CONN_SAXPARSERHANDLER_H_
#define _CONN_SAXPARSERHANDLER_H_

#include "Conn.h"

namespace conn
{

class CONN_EXPORT SaxParserHandler
{
public:
   typedef std::map<std::string,std::string> AttributeMap;

   virtual void onElementStart (const std::string &element, const AttributeMap &attributes, const std::string &file, uint64_t line) = 0;
   virtual void onElementEnd (const std::string &element, const std::string &data) = 0;

   bool startsWith (const std::string &str, const std::string &end);
   bool endsWith (const std::string &str, const std::string &end);
   void replace (std::string &str, const std::string &rep, const std::string &with);
   void trim (std::string &str);
};

}

#endif
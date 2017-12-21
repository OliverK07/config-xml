#include "SaxParserException.h"

namespace conn
{

static std::string makeMessage (const std::string &file, uint64_t line, uint64_t column, const std::string &message)
{
   std::ostringstream str;

   str << file << "[" << line << ":" << column << "] " << message;
   return str.str ();
}

SaxParserException::SaxParserException (const std::string &file, uint64_t line, uint64_t column, const std::string &message)
   : Exception (makeMessage (file, line, column, message))
{
}

}
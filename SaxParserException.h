#ifndef _CONN_SAXPARSEREXCEPTION_
#define _CONN_SAXPARSEREXCEPTION_

#include "Conn.h"
#include "Exception.h"

namespace conn
{

class CONN_EXPORT SaxParserException : public conn::Exception
{
public:
   SaxParserException (const std::string &file, uint64_t line, uint64_t column, const std::string &message);
};

}

#endif

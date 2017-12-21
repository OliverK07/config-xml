#ifndef _CONN_ENVIRONMENT_H_
#define _CONN_ENVIRONMENT_H_

#include "Conn.h"

namespace conn
{

class CONN_EXPORT Environment
{
public:
   static bool has (const std::string &name);
   static std::string get (const std::string &name, bool exc = true);
   static std::string expand (const std::string &str);
};

}

#endif
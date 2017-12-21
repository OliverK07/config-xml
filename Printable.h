#ifndef _CONN_PRINTABLE_H_
#define _CONN_PRINTABLE_H_

#include "Conn.h"

namespace conn
{

class CONN_EXPORT Printable
{
public:
   virtual std::ostream &print (std::ostream &os) const = 0;
};

}

CONN_EXPORT std::ostream &operator << (std::ostream &os, const conn::Printable &printable);

#endif
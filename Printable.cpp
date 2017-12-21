#include <typeinfo>
#include "Printable.h"

namespace conn
{

}

std::ostream &operator<< (std::ostream &os, const conn::Printable &printable)
{
   return printable.print (os);
}
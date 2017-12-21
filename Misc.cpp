#include "Conn.h"

namespace conn
{

Timestamp time_now()
{
   timespec ts;
   Timestamp r;

   clock_gettime (CLOCK_REALTIME, &ts);
   r = (Timestamp)(ts.tv_sec) * (Timestamp) 1000000000;
   r += (Timestamp)(ts.tv_nsec);
   return r;
}

}
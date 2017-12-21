#include <errno.h>
#include <string.h>
#include <string>
#include "os_time.h"

void os_nanosleep (const struct timespec * rqtp)
{
   struct timespec ts = *rqtp;
   int res;
   do
   {
      res = nanosleep( &ts, &ts );
   } while (res != 0 && errno == EINTR);

   if (res != 0)
   {
      throw "error from os_nanosleep" + std::string(strerror (errno));
   }
}

void os_usleep( const uint64_t interval )
{
   const uint32_t ns_per_ms = 1000;
   const uint32_t ms_per_s = 1000000;
   const struct timespec ts = { (time_t)(interval/ms_per_s), (long)((interval % ms_per_s) * ns_per_ms) } ;
   os_nanosleep (&ts);
}
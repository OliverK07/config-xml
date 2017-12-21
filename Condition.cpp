#include "Condition.h"

namespace conn
{

bool Condition::wait (Mutex &mutex, uint64_t timeout)
{
   timespec finish;

   int ret = clock_gettime (CLOCK_REALTIME, &finish);

   uint64_t tmp_nsecs = timeout + finish.tv_nsec;
   finish.tv_nsec = (tmp_nsecs % 1000000000);
   finish.tv_sec += tmp_nsecs / 1000000000;

   ret = pthread_cond_timedwait (&m_cond, mutex, &finish);
   switch (ret)
   {
      case 0:
      {
         return false;
      }
      case ETIMEDOUT:
      {
         return true;
      }
      default:
      {
         throw;
      }
   }
}

}
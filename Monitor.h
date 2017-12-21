#ifndef _CONN_MONITOR_H_
#define _CONN_MONITOR_H_

#include "Mutex.h"
#include "Condition.h"

namespace conn
{

class CONN_EXPORT Monitor : public Mutex
{
public:
   Monitor () {}
   ~Monitor () {}

   void wait ();
   bool wait (uint64_t timeout);
   void notify ();
   void broadcast ();

private:
   Condition m_cond;

   Monitor (const Monitor &);
   Monitor & operator = (const Monitor &);
};

inline void Monitor::wait ()
{
   m_cond.wait (*this);
}

inline bool Monitor::wait (uint64_t timeout)
{
   return m_cond.wait (*this, timeout);
}

inline void Monitor::notify ()
{
   m_cond.notify ();
}

inline void Monitor::broadcast ()
{
   m_cond.broadcast ();
}

}

#endif
#ifndef _CONN_CONDITION_H_
#define _CONN_CONDITION_H_

#include "Conn.h"
#include "Mutex.h"

namespace conn
{

class CONN_EXPORT Condition
{
public:
   Condition ();
   ~Condition ();

   void wait (Mutex &mutex);
   bool wait (Mutex &mutex, uint64_t timeout);

   void notify ();
   void broadcast ();

private:
   pthread_cond_t m_cond;

   Condition (const Condition &);
   Condition & operator = (const Condition &);
};

inline Condition::Condition ()
{
   pthread_cond_init (&m_cond, 0);
}

inline Condition::~Condition()
{
   pthread_cond_destroy (&m_cond);
}

inline void Condition::wait (Mutex &mutex)
{
   pthread_cond_wait (&m_cond, mutex);
}

inline void Condition::notify ()
{
   pthread_cond_signal (&m_cond);
}

inline void Condition::broadcast ()
{
   pthread_cond_broadcast (&m_cond);
}

}

#endif
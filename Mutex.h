#ifndef _CONN_MUTEX_H_
#define _CONN_MUTEX_H_

#include "Conn.h"

namespace conn
{

class CONN_EXPORT Mutex
{
public:
   Mutex ();
   ~Mutex ();

   void lock ();
   bool trylock ();
   void unlock ();

   inline operator pthread_mutex_t * () { return &m_mutex; }

protected:
   pthread_mutex_t m_mutex;

   Mutex (const Mutex &);
   Mutex & operator = (const Mutex &);
};

class CONN_EXPORT MutexLock
{
public:
   MutexLock (Mutex &mutex);
   MutexLock (Mutex &mutex, bool tryLock);
   virtual ~MutexLock ();

   bool locked ()  { return m_locked; }

private:
   Mutex &m_mutex;
   bool m_locked;
};

class CONN_EXPORT MutexUnlock
{
public:
   MutexUnlock (Mutex &mutex);
   virtual ~MutexUnlock ();

private:
   Mutex &m_mutex;
};

inline Mutex::Mutex()
{
   pthread_mutex_init (&m_mutex, 0);
}

inline Mutex::~Mutex ()
{
   pthread_mutex_destroy (&m_mutex);
}

inline void Mutex::lock ()
{
   int ret = pthread_mutex_lock (&m_mutex);
   assert (ret == 0);
   (void) ret;
}

inline bool Mutex::trylock ()
{
   int ret = pthread_mutex_trylock (&m_mutex);
   assert ((ret == 0) || (ret == EBUSY));
   return (ret == 0);
}

inline void Mutex::unlock ()
{
   int ret = pthread_mutex_unlock (&m_mutex);
   assert (ret == 0);
   (void) ret;
}

inline MutexLock::MutexLock (Mutex &mutex)
   : m_mutex (mutex), m_locked (false)
{
   m_mutex.lock ();
   m_locked = true;
}

inline MutexLock::MutexLock (Mutex &mutex, bool tryLock)
   : m_mutex (mutex), m_locked (false)
{
   if (tryLock)
   {
      m_locked = m_mutex.trylock ();
   }
   else
   {
      m_mutex.lock ();
      m_locked = true;
   }
}

inline MutexLock::~MutexLock ()
{
   if (locked ())
   {
      m_mutex.unlock ();
   }
}

inline MutexUnlock::MutexUnlock (Mutex &mutex)
   : m_mutex (mutex)
{
   m_mutex.unlock ();
}

inline MutexUnlock::~MutexUnlock ()
{
   m_mutex.lock ();
}

}

#endif
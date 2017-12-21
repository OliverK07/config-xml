#include "Counted.h"
#include "Mutex.h"
#include "Printable.h"
#include "Logger.h"

namespace conn
{
/**
 * For now just use one global mutex for all reference counting operations, and optimize later if needed.
 */
static Mutex g_mutex;

CountedBase::CountedBase ()
   : m_count (0)
{
}

CountedBase::~CountedBase ()
{
   if (m_count != 0)
   {
      Printable *p = dynamic_cast<Printable*> (this);

      if (p)
      {
         CONN_LOG_ERROR (this, "Destructing " << *p << " with m_count = " << m_count);
      }
      else
      {
         CONN_LOG_ERROR (this, "Destructing CountedBase object @ " << this << " with m_count = " << m_count);
      }
   }
}

void CountedBase::attach () const
{
   MutexLock lock (g_mutex);
   m_count++;
}

void CountedBase::detach () const
{
   bool zero;

   {
      MutexLock lock (g_mutex);
      zero = (--m_count == 0);
   }

   if (zero)
   {
      detached ();
   }
}

void CountedBase::detached () const
{
   delete this;
}

}
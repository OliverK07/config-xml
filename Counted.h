#ifndef __CONN_COUNTED_H__
#define __CONN_COUNTED_H__

#include "Conn.h"

namespace conn
{

class CONN_EXPORT Counted
{
public:
   virtual ~Counted () {}

   virtual void attach () const {}
   virtual void detach () const {}
};

class CONN_EXPORT CountedBase : public virtual Counted
{
public:
   CountedBase ();
   virtual ~CountedBase ();

   virtual void attach () const;
   virtual void detach () const;
   virtual void detached () const;

private:
   mutable uint32_t m_count;
};

typedef Handle<Counted> HCounted;

}

#endif
#ifndef _CONN_HANDLE_H_
#define _CONN_HANDLE_H_

namespace conn
{

template <class T>
class Handle
{
public:
   Handle ();
   Handle (const Handle<T> &other);
   Handle (T *ptr);
   virtual ~Handle ();

   T * operator-> () const { return m_ptr; }
   T * ptr () const { return m_ptr; }
   T & obj () const { assert (m_ptr); return *m_ptr; }

   T * attach () const;

   Handle<T> & operator= (const Handle<T> &rhs);
   Handle<T> & operator= (T *rhs);

   bool operator== (const Handle<T> &rhs) const { return m_ptr == rhs.m_ptr; }
   bool operator!= (const Handle<T> &rhs) const { return m_ptr != rhs.m_ptr; }
   bool operator< (const Handle<T> &rhs) const { return m_ptr < rhs.m_ptr; }

   bool operator== (const T *rhs) const { return m_ptr == rhs; }
   bool operator!= (const T *rhs) const { return m_ptr != rhs; }

   bool operator! () const { return !m_ptr; }

private:
   T *m_ptr;
};

template<class T>
Handle<T>::Handle ()
   : m_ptr (0)
{
}

template<class T>
Handle<T>::Handle (const Handle<T> &other)
   : m_ptr (other.m_ptr)
{
   if (m_ptr)
   {
      m_ptr->attach ();
   }
}

template<class T>
Handle<T>::Handle (T *ptr)
   : m_ptr (ptr)
{
   if (m_ptr)
   {
      m_ptr->attach ();
   }
}

template<class T>
Handle<T>::~Handle ()
{
   if (m_ptr)
   {
      m_ptr->detach ();
      m_ptr = 0;
   }
}

template<class T>
T * Handle<T>::attach () const
{
   m_ptr->attach ();
   return m_ptr;
}

template<class T>
Handle<T> & Handle<T>::operator= (const Handle<T> &rhs)
{
   return this->operator= (rhs.m_ptr);
}

template<class T>
Handle<T> & Handle<T>::operator= (T *rhs)
{
   if (m_ptr)
   {
      m_ptr->detach ();
   }
   m_ptr = rhs;
   if (m_ptr)
   {
      m_ptr->attach ();
   }
   return *this;
}

template<class T>
bool operator== (const T *lhs, const Handle<T> &rhs)
{
   return lhs == rhs.ptr ();
}

template<class T>
bool operator!= (const T *lhs, const Handle<T> &rhs)
{
   return lhs != rhs.ptr ();
}

}

#endif
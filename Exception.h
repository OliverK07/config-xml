#ifndef _CONN_EXCEPTION_H_
#define _CONN_EXCEPTION_H_

#include "Conn.h"

namespace conn
{

class CONN_EXPORT Exception : public std::exception
{
public:
   Exception ();
   Exception (const std::string &what);
   ~Exception () throw ();

   const char * what () const throw ();

protected:
   virtual std::string getwhat () const;

private:
   mutable std::string m_what;
};

#define DEFINE_CONN_EXCEPTION(E)                       \
class CONN_EXPORT E : public ::conn::Exception         \
{                                                      \
public:                                                \
   E () {}                                             \
   E (const std::string &what) : Exception (what) {}   \
};

DEFINE_CONN_EXCEPTION (PropertyNotExistException);
DEFINE_CONN_EXCEPTION (ValueNotExistException);

DEFINE_CONN_EXCEPTION (WrongSateException);
DEFINE_CONN_EXCEPTION (WrongTypeException);
DEFINE_CONN_EXCEPTION (WrongCardinalityException);

DEFINE_CONN_EXCEPTION (InvalidEventIdentifier);
DEFINE_CONN_EXCEPTION (InitializationFailedException);

DEFINE_CONN_EXCEPTION (DuplicateEventIdentifier);
DEFINE_CONN_EXCEPTION (SchedulerStartError);

DEFINE_CONN_EXCEPTION (UnhandledTypeException);

CONN_EXPORT std::string current_exception_to_string ();
CONN_EXPORT std::string current_exception_to_xml_string ();

}

#endif
#include <typeinfo>
#if defined (os_linux_gcc_x86) || (os_linux_gcc_arm) || (os_linux_gcc_x64)
#include <cxxabi.h>
#endif
#include "Exception.h"

namespace conn
{

Exception::Exception ()
{
}

Exception::Exception (const std::string &what)
   : m_what (what)
{
}

Exception::~Exception () throw ()
{
}

const char * Exception::what () const throw ()
{
   if (m_what.empty ())
   {
      m_what = getwhat ();
   }

   return m_what.c_str ();
}

std::string Exception::getwhat () const
{
   return "";
}

std::string current_exception_to_string ()
{
   std::stringstream ss;

   try
   {
      throw;
   }
   catch (int i)
   {
      ss << "int(" << i << ")";
   }
   catch (const char *s)
   {
      ss << "\"" << s << "\"";
   }
   catch (const std::string &s)
   {
      ss << "\"" << s << "\"";
   }
   catch (const std::string *s)
   {
      ss << "\"" << (*s) << "\"";
   }
   catch (const std::exception *e)
   {
      ss << typeid(*e).name() << "(\""<< e->what() << "\")";
   }
   catch (const std::exception &e)
   {
      ss << typeid(e).name() << "(\""<< e.what() << "\")";
   }
   catch (...)
   {
      #if defined (os_linux_gcc_x86) || (os_linux_gcc_arm) || (os_linux_gcc_x64)
      ss << __cxxabiv1::__cxa_current_exception_type()->name();
      #else
      ss << "Unknown exception";
      #endif
   }

   return ss.str ();
}

std::string current_exception_to_xml_string ()
{
   std::string src = current_exception_to_string ();
   std::string result;
   result.reserve (src.size ());

   for (std::string::iterator i = src.begin(); i != src.end(); ++i)
   {
      if (*i == '"')
      {
         result += "&quot;";
      }
      else
      {
         result += *i;
      }
   }
   return result;
}

}
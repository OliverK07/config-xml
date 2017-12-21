#include "Environment.h"
#include "Exception.h"

namespace conn
{

bool Environment::has (const std::string &name)
{
   return (get (name, false).size () != 0);
}

std::string Environment::get (const std::string &name, bool exc)
{
   char *envVar = ::getenv (name.c_str ());
   std::string val;

   if (NULL != envVar)
   {
      val = envVar;
   }
   else if (exc)
   {
      throw conn::Exception (std::string ("Environment variable ") + name + " not set");
   }
   return val;
}

std::string Environment::expand (const std::string &str)
{
   std::string result;
   std::string::const_iterator iter;

   for (iter = str.begin (); iter != str.end (); iter++)
   {
      if (*iter == '$')
      {
         std::string varname ("");
         iter++;
         if (iter != str.end () && (::isalpha (*iter) || *iter == '_'))
         {
            for (;iter != str.end () && (::isalnum (*iter) || *iter == '_'); iter++)
            {
               varname += *iter;
            }
         }
         iter--;
         if (varname.length () == 0)
         {
            result += "$";
         }
         else
         {
            try
            {
               result += get (varname);
            }
            catch (conn::Exception &e)
            {
               throw e;
            }
         }
      }
      else
      {
         result += *iter;
      }
   }
   return result;
}

}
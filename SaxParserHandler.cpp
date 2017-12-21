#include "SaxParserHandler.h"

namespace conn
{

bool SaxParserHandler::startsWith (const std::string &str, const std::string &start)
{
   return (str.find (start, 0) == 0);
}

bool SaxParserHandler::endsWith (const std::string &str, const std::string &end)
{
   return (str.find (end, str.size () - end.size ()) != str.npos);
}

void SaxParserHandler::replace (std::string &str, const std::string &rep, const std::string &with)
{
   std::size_t found;
   while ((found = str.rfind (rep)) != std::string::npos)
   {
      str.replace (found, rep.length (), with);
   }
}

void SaxParserHandler::trim (std::string &str)
{
   static char whitespaces[] = " \t\r\n\v\f";
   bool isWhitespace = (str.find_first_not_of (whitespaces) == std::string::npos);
   if (isWhitespace) {
      std::size_t found = str.find_last_not_of (whitespaces);

      if (found != std::string::npos)
      {
         str.erase (found + 1);
      }
      else
      {
         str.clear ();
      }
   }
}

}


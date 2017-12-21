#include <limits.h>
#include <sys/stat.h>
#include <algorithm>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#include "Path.h"
#include "Logger.h"
#include "Exception.h"
#include "Environment.h"

#define OS_MAX_PATH PATH_MAX
const char SEPARATOR = '/';

namespace conn
{

Path::Path (Logger *logger)
   : m_logger (logger ? logger : Logger::root ())
{
}

Path::Path (const Path &path, Logger *logger)
   : m_path (path.m_path), m_logger (logger ? logger : path.m_logger)
{
   std::replace (m_path.begin (), m_path.end (), '/', SEPARATOR);
}

Path::Path (const Path & path1, const std::string & path2, Logger * logger)
   : m_path (path1.m_path + '/' + path2), m_logger (logger ? logger : path1.m_logger)
{
   std::replace (m_path.begin (), m_path.end (), '/', SEPARATOR);
}

Path::Path (const char * path, Logger * logger)
   : m_path (path), m_logger (logger ? logger : Logger::root ())
{
   std::replace (m_path.begin (), m_path.end (), '/', SEPARATOR);
}

Path::Path (const std::string & path, Logger * logger)
   : m_path (path), m_logger (logger ? logger : Logger::root ())
{
   std::replace (m_path.begin (), m_path.end (), '/', SEPARATOR);
}

Path::~Path ()
{
}

Path & Path::operator = (const Path &rhs)
{
   return operator= (rhs.c_str ());
}

Path & Path::operator = (const std::string &rhs)
{
   return operator= (rhs.c_str ());
}

Path & Path::operator = (const char *rhs)
{
   m_logger = Logger::root ();
   m_path = rhs;
   std::replace (m_path.begin (), m_path.end (), '/', SEPARATOR);
   return *this;
}

Path Path::operator / (const std::string &rhs) const
{
   Path result (*this);

   result.m_path += SEPARATOR;
   result.m_path += rhs;
   return result;
}

Path &Path::operator /= (const std::string &rhs)
{
   m_path += SEPARATOR;
   m_path += rhs;

   return *this;
}

Path Path::removeExtension () const
{
   std::string::size_type firstdot = m_path.find_first_of ('.');

   if (firstdot == std::string::npos)
   {
      return *this;
   }
   else
   {
      return Path (m_path.substr (0, firstdot));
   }
}

Path Path::expand (const std::string &str)
{
   return Environment::expand (str);
}

std::string Path::read () const
{
   (void) isReadableFile (true);

   std::string content;
   std::ifstream str (c_str ());

   str.seekg (0, std::ios::end);
   content.reserve (str.tellg ());
   str.seekg (0, std::ios::beg);

   content.assign ((std::istreambuf_iterator<char> (str)), std::istreambuf_iterator<char>());
   return content;
}

void Path::write (const std::string &content) const
{
   std::ofstream str (c_str ());
   str << content;
   str.close ();
}

Path Path::getCwd ()
{
   char result [OS_MAX_PATH];
   bool gotPath = false;

   gotPath = (::getcwd (result, sizeof (result)) != 0);
   if (gotPath && strlen (result) > 0)
   {
      return Path (result);
   }
   CONN_LOG_ERROR_THROW (Logger::root (), conn::Exception, std::string ("::getcwd ()  : ") + strerror (errno));
}

Path Path::getRealPath () const
{
   char result [OS_MAX_PATH];

   if (::realpath (c_str (), result) == 0)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception, "Error accessing '" + m_path + "' : " + strerror (errno));
   }
   return Path (result, m_logger);
}

Path Path::getDirName () const
{
   char copy [OS_MAX_PATH];
   strncpy (copy, c_str (), OS_MAX_PATH - 1);
   copy [OS_MAX_PATH - 1] = '\0';
   return Path (::dirname (copy)); // dirname may modify the input string so we use a copy
}

Path Path::getBaseName () const
{
   char copy [OS_MAX_PATH];
   strncpy (copy, c_str (), OS_MAX_PATH - 1);
   copy [OS_MAX_PATH - 1] = '\0';
   return Path (::basename (copy)); // dirname may modify the input string so we use a copy
}

void Path::chdir () const
{
   int changed = -1;

   changed = ::chdir (c_str ());
   if (changed == -1)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception, "chdir (" << *this << ") : " << strerror (errno));
   }
}

void Path::mkDirs () const
{
   char pathname [OS_MAX_PATH];
   const char *src;
   uint32_t len, pos;
   Path dirname (getDirName ());

   src = dirname.c_str ();
   len = strlen (src);
   memset (pathname, 0, sizeof (pathname));
   for (pos = 0; pos <= len; pos++)
   {
      if (src[pos] == SEPARATOR || pos == len)
      {
         // Don't attempt to create filesystem root, i.e. '/'
         if (pos != 0)
         {
            if (mkdir (pathname, S_IRWXU) != 0 && errno != EEXIST)
            {
               // caller decides what happens on directory/ or not a directory being already there
               CONN_LOG_ERROR_THROW (m_logger, conn::Exception , std::string ("Path['") + pathname + "'] cannot be created : " + strerror (errno));
            }
         }
      }
      pathname[pos] = src[pos];
   }
}

bool Path::isAbsolute (bool exception) const
{
   bool isAbFile = false;

   isAbFile = ((m_path.length () > 0) && (m_path [0] == SEPARATOR));
   if (!isAbFile && exception)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception, "Path['" + m_path + "'] is not absolute");
   }
   return isAbFile;
}

bool Path::isFile (bool exception) const
{
   bool file = false;
   struct stat s;

   file = ((::stat (m_path.c_str (), &s) == 0) && S_ISREG (s.st_mode));
   if (!file && exception)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception, "'" << m_path << "' is not a file");
   }
   return file;
}

bool Path::isDirectory (bool exception) const
{
   bool isDir = false;
   struct stat s;

   isDir = ((::stat (m_path.c_str (), &s) == 0) && S_ISDIR (s.st_mode));
   if (!isDir && exception)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception, "'" << m_path << "' is not a directory");
   }
   return isDir;
}

bool Path::isReadableFile (bool exception) const
{
   bool isRFile = false;

   isRFile = (::access (c_str (), R_OK) == 0);
   if (!isRFile && exception)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception, "Path['" + m_path + "'] is not a readable file : " + strerror (errno));
   }
   return isRFile;
}

bool Path::isWritableFile (bool exception) const
{
   bool isWFile = false;

   isWFile = (::access (c_str (), W_OK) == 0);
   if (!isWFile && exception)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception , "Path['" + m_path + "'] is not a writable file : " + strerror (errno));
   }
   return isWFile;
}

bool Path::isExecutableFile (bool exception) const
{
   bool isExecFile = false;

   isExecFile = (::access (c_str (), X_OK) == 0);
   if (!isExecFile && exception)
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception , "Path['" + m_path + "'] is not an executable file : " + strerror (errno));
   }
   return isExecFile;
}

/*returns a list of paths , can be empty the caller decides what happens on empty list*/
const std::vector<conn::Path> Path::listDirectory () const
{
   std::vector <conn::Path> dirs;
   DIR * dirPtr = ::opendir (m_path.c_str ());

   if (NULL != dirPtr)
   {
      struct dirent * indvDirs;
      while (0 != (indvDirs = ::readdir (dirPtr)))
      {
         if ((::strcmp (indvDirs->d_name, ".") != 0) && (::strcmp (indvDirs->d_name , "..") != 0))
         {
            dirs.push_back (m_path + SEPARATOR + indvDirs->d_name);
         }
      }
      if (0 != ::closedir (dirPtr))
      {
         CONN_LOG_ERROR (m_logger, "Unable to close directory " << m_path);
      }
   }
   else
   {
      CONN_LOG_ERROR_THROW (m_logger, conn::Exception, " failed to open directory at " << m_path);
   }

   return dirs;
}

}

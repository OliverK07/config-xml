#ifndef __CONN_PATH_H__
#define __CONN_PATH_H__

#include "Conn.h"

namespace conn
{

class CONN_EXPORT Path
{
public:
   Path (Logger *logger = 0);
   Path (const Path &path, Logger *logger = 0);
   Path (const Path &path1, const std::string &path2, Logger *logger = 0);
   Path (const char *path, Logger *logger = 0);
   Path (const std::string &path, Logger *logger = 0);
   ~Path ();

   Path & operator = (const Path &rhs);
   Path & operator = (const std::string &rhs);
   Path & operator = (const char *rhs);
   Path operator / (const std::string &rhs) const;
   Path &operator /= (const std::string &rhs);
   std::ostream & operator << (std::ostream &os) const { return os << m_path; }

   operator const std::string & () const { return m_path; }
   operator const char * () const { return m_path.c_str (); }

   const std::string & str () const { return m_path; }
   const char * c_str () const { return m_path.c_str (); }

   Path removeExtension () const;

   static Path expand (const std::string &str);

   // Returns the contents of the file, named by this path, as a string
   std::string read () const;

   // Writes the specified string content into the file named by this path,
   // creating the file if it doesn't already exist.
   void write (const std::string &content) const;

   // <a href="http://pubs.opengroup.org/onlinepubs/009695399/functions/getcwd.html"/>
   static Path getCwd ();

   // <a href="http://pubs.opengroup.org/onlinepubs/009695399/functions/realpath.html"/>
   Path getRealPath () const;

   // <a href="http://pubs.opengroup.org/onlinepubs/009695399/functions/dirname.html"/>
   Path getDirName() const;

   // <a href="http://pubs.opengroup.org/onlinepubs/009695399/functions/basename.html"/>
   Path getBaseName() const;

   // <a href="http://pubs.opengroup.org/onlinepubs/009695399/functions/chdir.html"/>
   void chdir () const;

   // Attempts to create getDirName() as a directory if it doesnt exist
   void mkDirs () const;

   // returns a list of paths
   const std::vector <conn::Path> listDirectory () const;

   bool isAbsolute (bool exception = false) const;
   bool isFile (bool exception = false) const;
   bool isDirectory (bool exception = false) const;
   bool isReadableFile (bool exception = false) const;
   bool isWritableFile (bool exception = false) const;
   bool isExecutableFile (bool exception = false) const;

private:
   std::string m_path;
   Logger *m_logger;
};

}

#endif
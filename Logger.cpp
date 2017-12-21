#include "Logger.h"
#include "Environment.h"

namespace conn
{

static Logger::Level DEFAULT_LEVEL = Logger::CONN_LOGGER_WARN;
static char LEVEL_CHARS[5] = { 'D', 'I', 'W', 'E', 'X' };
static const bool COLOURED = conn::Environment::has ("CONN_LOGGER_COLOURED");
static const char *COLOURS[5] = { "\033[34m", "\033[37m", "\033[36m", "\033[31m", "" };
static std::ostream *m_stream = &std::cout; /*default stream*/
static std::fstream *m_fstream = 0;

Logger * Logger::root ()
{
   static Logger ROOT;
   return &ROOT;
}

Logger::Logger ()
   : m_parent (0), m_name (""), m_fullname (""), m_level (DEFAULT_LEVEL), m_children ()
{
}

Logger::Logger (Logger *parent, const std::string &name)
   : m_parent (parent), m_name (name),
     m_fullname ((parent->m_fullname == "") ? name : parent->m_fullname + "/" + name),
     m_level (CONN_LOGGER_INHERIT), m_children ()
{
}

Logger::~Logger ()
{
   if (0 != m_fstream)
   {
      m_fstream->close ();
      delete (m_fstream);
   }

   m_stream = 0;
   m_fstream = 0;

   for (ChildrenMap::iterator i = m_children.begin (); m_children.size () > 0; i = m_children.begin ())
   {
      delete i->second;
      i->second = 0;
      m_children.erase (i);
   }
   m_parent = 0;
}

const std::string & Logger::getName () const
{
   return m_name;
}

const std::string & Logger::getFullName () const
{
   return m_fullname;
}

void Logger::setLevel (Logger::Level level)
{
   switch (level)
   {
      case CONN_LOGGER_DEBUG:
      case CONN_LOGGER_INFO:
      case CONN_LOGGER_WARN:
      case CONN_LOGGER_ERROR:
      case CONN_LOGGER_INHERIT:
          m_level = level; break;
      default: m_level = DEFAULT_LEVEL; break;
   }
}

Logger::Level Logger::getLevel ()
{
   return m_level;
}

Logger::Level Logger::stringToLevel (const std::string &level)
{
   if (level == "DEBUG") return Logger::CONN_LOGGER_DEBUG;
   if (level == "INFO") return Logger::CONN_LOGGER_INFO;
   if (level == "WARN") return Logger::CONN_LOGGER_WARN;
   if (level == "ERROR") return Logger::CONN_LOGGER_ERROR;
   if (level == "INHERIT") return Logger::CONN_LOGGER_INHERIT;
   throw std::string ("Invalid Logger::Level string '" + level + "'");
}

std::string Logger::leveltoString (Level level)
{
   switch (level)
   {
      case CONN_LOGGER_DEBUG: return "DEBUG";
      case CONN_LOGGER_INFO: return "INFO";
      case CONN_LOGGER_WARN: return "WARN";
      case CONN_LOGGER_ERROR: return "ERROR";
      case CONN_LOGGER_INHERIT: return "INHERIT";
      default: return "????";
   }
}

void Logger::setLog (const std::string &logFile)
{
   if (!logFile.empty () && logFile != "")
   {
      m_fstream = new std::fstream (logFile.c_str (), std::fstream::out | std::fstream::app);
      if (!m_fstream->is_open ())
      {
         delete m_fstream;
         m_fstream = 0;
         throw  ("failed to open " + logFile + " for logging");
      }
      else
      {
         m_stream = m_fstream;
      }
   }
}

Logger * Logger::getChild (const std::string &name)
{
   Logger *child = m_children[name];

   if (child == 0)
   {
      child = new Logger (this, name);
      m_children[name] = child;
   }

   return child;
}

void Logger::write (Level level, const  std::string &msg) const
{
   if (enabled (level))
   {
      char name[64];
#ifdef __GLIBC__
      pthread_getname_np (pthread_self (), name, sizeof (name));
#else
      pthread_t id = pthread_self ();
      snprintf (name, sizeof (name), "%p", (void*) &id);
#endif

      std::ostringstream str;

      str << (COLOURED ? COLOURS[level] : "") << time_now () << "<" << LEVEL_CHARS[level] << ">:" << name;
      if (m_fullname.length ())
      {
         str << ":" << m_fullname;
      }
      str << ": " << msg << (COLOURED ? "\033[0m" : "") << std::endl;

      // avoid two .str () calls
      std::string tempStrng = str.str ();
      m_stream->write (tempStrng.c_str (), tempStrng.size ());
      m_stream->flush ();
   }
}

}
#ifndef _CONN_LOGGER_H_
#define _CONN_LOGGER_H_

#include "Conn.h"

namespace conn
{

class CONN_EXPORT Logger
{
public:
   enum Level { CONN_LOGGER_DEBUG = 0, CONN_LOGGER_INFO, CONN_LOGGER_WARN, CONN_LOGGER_ERROR, CONN_LOGGER_INHERIT };

   static Logger * root ();

   const std::string & getName () const;
   const std::string & getFullName () const;

   void setLevel (Logger::Level level);
   Logger::Level getLevel ();

   static Level stringToLevel (const std::string &level);
   static std::string leveltoString (Level level);

   static void setLog (const std::string &logFile);
   Logger * getChild (const std::string &name);
   virtual void write (Level level, const  std::string &msg) const;

   bool enabled (Level level) const;
   static Logger * get (const LoggerProvider *context);
   static Logger * get (const char *context);
   static Logger * get (Logger *context);
   template<class T> static Logger * get (T *context);
   template<class T> static Logger * get (T &context);

private:
   Logger ();
   Logger (const Logger&);
   Logger (Logger *parent, const std::string &name);
   virtual ~Logger ();

   void operator= (const Logger&);

   typedef std::map<std::string,Logger*> ChildrenMap;

   Logger*     m_parent;
   std::string m_name;
   std::string m_fullname;
   Level       m_level;
   ChildrenMap m_children;
};

class CONN_EXPORT LoggerProvider
{
public:
   virtual Logger * getLogger () const { return Logger::root (); }
};

inline bool Logger::enabled (Level level) const
{
   return (m_level == CONN_LOGGER_INHERIT) ? m_parent->enabled (level) : (level >= m_level);
}

inline Logger * Logger::get (const char *context)
{
   return Logger::root ();
}

inline Logger * Logger::get (Logger *context)
{
   return context;
}

template<class T> Logger * Logger::get (T *context)
{
   const LoggerProvider *provider = dynamic_cast<const LoggerProvider*> (context);
   return provider ? provider->getLogger() : Logger::root ();
}

template<class T> Logger * Logger::get (T &context)
{
   return get<T> (&context);
}

}


#define CONN_LOG_MSG(context,level,msg)                                    \
{                                                                          \
   try                                                                     \
   {                                                                       \
      const conn::Logger *__l = conn::Logger::get (context);               \
      if (__l && (__l->enabled (level)))                                   \
      {                                                                    \
         std::ostringstream __ss;                                          \
         __ss << msg;                                                      \
         __l->write (level, __ss.str ());                                  \
      }                                                                    \
   }                                                                       \
   catch (...)                                                             \
   {                                                                       \
      std::cerr << "CONN_LOG_MSG(...) raised an exception" << std::endl;   \
   }                                                                       \
}

#define CONN_LOG_DEBUG(context,msg) CONN_LOG_MSG(context,conn::Logger::CONN_LOGGER_DEBUG,msg)
#define CONN_LOG_INFO(context,msg)  CONN_LOG_MSG(context,conn::Logger::CONN_LOGGER_INFO,msg)
#define CONN_LOG_WARN(context,msg)  CONN_LOG_MSG(context,conn::Logger::CONN_LOGGER_WARN,msg)
#define CONN_LOG_ERROR(context,msg) CONN_LOG_MSG(context,conn::Logger::CONN_LOGGER_ERROR,msg)

#define CONN_LOG_ERROR_THROW(context,type,msg)   \
{                                                \
   CONN_LOG_ERROR (context,msg);                 \
   std::ostringstream __ss;                      \
   __ss << msg;                                  \
   throw type (__ss.str ());                     \
}

#define CONN_LOG_ERROR_EXIT(context,msg)   \
{                                          \
   CONN_LOG_ERROR (context,msg);           \
   exit (-1);                              \
}

#endif
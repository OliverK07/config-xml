#ifndef _CONN_H_
#define _CONN_H_

#include "os.h"
#include "os_time.h"
#include "DLLExport.h"

namespace conn
{

class Mutex;
class MutexLock;
class MutexUnlock;
class Condition;
class Monitor;

class Exception;
class Environment;
class Path;

class Logger;
class LoggerProvider;

class Printable;
class Counted;
class CountedBase;

class SaxParser;
class SaxParserHandler;
class SaxParserException;

class Configuration;
class ConfigurationException;


template<class T> class Handle;

typedef Handle<Configuration> HConfiguration;

typedef uint64_t Timestamp;
typedef uint64_t Timestamp;
CONN_EXPORT Timestamp time_now ();

}

#endif
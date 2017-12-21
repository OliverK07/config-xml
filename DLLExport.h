#ifndef _CONN_DLLEXPORT_H_
#define _CONN_DLLEXPORT_H_

#undef CONN_EXPORT
#if defined _WIN32
   #if defined CONN_BUILD_CONN_DLL
      #define CONN_EXPORT __declspec (dllexport)
   #else
      #define CONN_EXPORT __declspec (dllimport)
   #endif
#else
   #define CONN_EXPORT
#endif

#endif
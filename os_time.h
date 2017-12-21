#ifndef _CONNECT_OS_OS_TIME_H_
#define _CONNECT_OS_OS_TIME_H_

#include <time.h>
#include <stdint.h>
#include "DLLExport.h"

/* nanosleep inplementation which is not interuppted by EINTR */
/* may throw exception for invalid values of rqtp */
CONN_EXPORT void os_nanosleep (const struct timespec * rqtp);
CONN_EXPORT void os_usleep( const uint64_t delay);

#endif

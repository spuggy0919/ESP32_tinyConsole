

#ifndef __MPI_debug_H__
#define __MPI_debug_H__

#define _TINYCONSOLE_
#ifdef _TINYCONSOLE_
#include "ESP32inc.h"
#else
#include <Arduino.h>
#endif
#include <mutex>
#include <queue>

// Define debug levels
#define MPI_DBG_ERROR   (1 << 0) // Bit 0 for error messages
#define MPI_DBG_WARNING (1 << 1) // Bit 1 for warning messages
#define MPI_DBG_INFO    (1 << 2) // Bit 2 for info messages
#define MPI_DBG_UDP     (1 << 3) // HANDLE 
#define MPI_DBG_MSG     (1 << 4) // HANDLE 
#define MPI_DBG_API     (1 << 5) // HANDLE 
#define MPI_NO_DEBUG    0x0000 // debug

// Define a variable to hold the debug level
extern uint16_t MPI_debugLevel; // Enable error and warning messages
extern std::mutex MPI_debugmutex;
// Macro to check if a specific bit is set in the debug level
#define IS_MPI_DBG_ENABLED(level) ((MPI_debugLevel & (level)) != 0)

// Macro for printing debug messages based on the debug level
#ifdef _TINYCONSOLE_

#define MPI_PRINTF( fmt, ...) \
    do { \
        wsTextPrintf(fmt, ##__VA_ARGS__); \
    } while (0)
#define MPI_DBG_PRINTF(level, fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(level)) { \
            wsTextPrintf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_ERROR_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_ERROR)) { \
            wsTextPrintf("[MPI]ERROR:"); \
            wsTextPrintf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_WARNING_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_WARNING)) { \
            wsTextPrintf("[MPI]WARNING:"); \
            wsTextPrintf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_INFO_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_INFO)) { \
            wsTextPrintf("[MPI]"); \
            wsTextPrintf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_UDP_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_UDP)) { \
            wsTextPrintf("[MPI][UDP]"); \
            wsTextPrintf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_MSG_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_MSG)) { \
            wsTextPrintf("[MPI][MSG]"); \
            wsTextPrintf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_API_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_API)) { \
            wsTextPrintf("[MPI][API]"); \
            wsTextPrintf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#else
#define MPI_PRINTF( fmt, ...)  Serial.printf(fmt, ##__VA_ARGS__); 

#define MPI_DBG_PRINTF(level, fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(level)) { \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_ERROR_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_ERROR)) { \
            Serial.printf("[MPI]ERROR:"); \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_WARNING_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_WARNING)) { \
            Serial.printf("[MPI]WARNING:"); \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_INFO_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_INFO)) { \
            Serial.printf("[MPI]"); \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_UDP_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_UDP)) { \
            Serial.printf("[MPI][UDP]:"); \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_MSG_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_MSG)) { \
            Serial.printf("[MPI][MSG]"); \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_API_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_API)) { \
            Serial.printf("[MPI][API]"); \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#endif


// #define MPI_DEBUGCHK_SET(x)  if((MPI_debugLevel&(x))!=0) 
// #define MPI_DEBUGCHK_CLR(x)  if((MPI_debugLevel&(x))==0) 
// handle debug level
#define MPI_DBG_CHK_RET_VOID(x)  {if ((MPI_debugLevel&(x))==0) return;}
#define MPI_DBG_CHK_RET_VALUE(x,value)  {if ((MPI_debugLevel&(x))==0) return (value);}

#endif //__MPI_debug_H__


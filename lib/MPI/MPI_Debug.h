

#ifndef __MPI_debug_H__
#define __MPI_debug_H__

#include <mutex>
#include <queue>   // TODO on mutex maybe remove

#define _TINYCONSOLE_
// Macro for printing debug messages based on the debug level
// this is for ESPTinyCONSOLE project , use websocket for serial
#ifdef _TINYCONSOLE_
void wsTextPrintf(const char *fmt,...); // tinyconsolee websocket

#define MPI_PRINTF( fmt, ...) \
    do { \
        wsTextPrintf(fmt, ##__VA_ARGS__); \
    } while (0)
#define MPI_MPRINTF( fmt, ...) \
    do { \
        wsTextPrintf(fmt, ##__VA_ARGS__); \
    } while (0)
#else
#include <Arduino.h>
#define MPI_PRINTF( fmt, ...)  Serial.printf(fmt, ##__VA_ARGS__); 
#define MPI_MPRINTF( fmt, ...)  Serial.printf(fmt, ##__VA_ARGS__); 
#endif


// Define debug levels
#define MPI_DBG_ERROR   (1 << 0) // Bit 0 for error messages
#define MPI_DBG_WARNING (1 << 1) // Bit 1 for warning messages
#define MPI_DBG_INFO    (1 << 2) // Bit 2 for info messages
#define MPI_DBG_UDP     (1 << 3) // HANDLE 
#define MPI_DBG_MSG     (1 << 4) // HANDLE 
#define MPI_DBG_API     (1 << 5) // HANDLE 
#define MPI_DBG_PINGPONG (1 << 7) // HANDLE 
#define MPI_DBG_UDPDUMP  (1 << 8) // HANDLE 
#define MPI_NO_DEBUG    0x0000 // debug

// Define a variable to hold the debug level
extern uint16_t MPI_debugLevel; // Enable error and warning messages

// Macro to check if a specific bit is set in the debug level
#define IS_MPI_DBG_ENABLED(level) ((MPI_debugLevel & (level)) != 0)


#define MPI_DBG_FOREVER_DEAD() \
    do { \
             int i=0;\
             while(1){\
                MPI_Iot_LED(i);\
                delay(500);\
                i=255-i;\
             }\
    }while (0);

#define MPI_DBG_PINGPONG_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_PINGPONG)) { \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_PRINTF(level, fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(level)) { \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_ERROR_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_ERROR)) { \
            MPI_MPRINTF("[MPI]ERROR:"); \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_WARNING_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_WARNING)) { \
            MPI_MPRINTF("[MPI]WARNING:"); \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)
#define MPI_DBG_INFO_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_INFO)) { \
            MPI_MPRINTF("[MPI]"); \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_UDP_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_UDP)) { \
            MPI_MPRINTF("[MPI][UDP]"); \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_MSG_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_MSG)) { \
            MPI_MPRINTF("[MPI][MSG]"); \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#define MPI_DBG_API_PRINTF( fmt, ...) \
    do { \
        if (IS_MPI_DBG_ENABLED(MPI_DBG_API)) { \
            MPI_MPRINTF("[MPI][API]"); \
            MPI_MPRINTF(fmt, ##__VA_ARGS__); \
        } \
    } while (0)


// #define MPI_DEBUGCHK_SET(x)  if((MPI_debugLevel&(x))!=0) 
// #define MPI_DEBUGCHK_CLR(x)  if((MPI_debugLevel&(x))==0) 
// handle debug level
#define MPI_DBG_CHK_RET_VOID(x)  {if ((MPI_debugLevel&(x))==0) return;}
#define MPI_DBG_CHK_RET_VALUE(x,value)  {if ((MPI_debugLevel&(x))==0) return (value);}

#endif //__MPI_debug_H__


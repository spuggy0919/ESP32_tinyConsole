#ifndef __DEBUGLOG__H__
#define __DEBUGLOG__H__
#include <Arduino.h>

/* 
   version 0.1
Fpr verb debug log  with level
*/

#define  __DEBUG_LEVEL__ 0
#define __DEBUG_ERROR__ -1

#define  __DEBUG__ 1
#if (__DEBUG__ > __DEBUG_LEVEL__)
#define DEBUG_PRINTLN(...) if (__DEBUG__>__DEBUG_LEVEL__) {Serial.println(__VA_ARGS__);}
#define DEBUG_PRINTF(...) if (__DEBUG__>__DEBUG_LEVEL__) {Serial.printf(__VA_ARGS__);}
#else
#define DEBUGPRINTLN(arg)  do{ } while ( __DEBUG_LEVEL__ )
#endif

#endif
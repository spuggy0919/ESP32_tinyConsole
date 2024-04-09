

/* 
   version 0.1
Fpr verb debug log  with level
to be defined
*/
#ifndef __DEBUGLOG__H__
#define __DEBUGLOG__H__
#include <Arduino.h>

#define  __DEBUG_LEVEL__ 0
#define __DEBUG_ERROR__ -1
#define __DEBUG_DRIVER__ 9

#define  __DEBUG__ 1
#if (__DEBUG__ > __DEBUG_LEVEL__)
#define DEBUG_PRINTLN(...) if (__DEBUG__<__DEBUG_LEVEL__) {Serial.println(__VA_ARGS__);}
#define DEBUG_PRINTF(...) if (__DEBUG__<__DEBUG_LEVEL__) {Serial.printf(__VA_ARGS__);}
#define DEBUGCHK(level)  if (level<__DEBUG_LEVEL__) 
#else
#define DEBUG_PRINTLN(...)    
#define DEBUG_PRINTF(...)     
#define DEBUGCHK(level)    
#endif

#if (__DEBUG_DRIVER__  <  __DEBUG_LEVEL__)
#define DEBUGDRVCHK(...)   {Serial.printf(__VA_ARGS__);}
#else
#define DEBUGDRVCHK(...)      
#endif



#define  __WSDEBUG_LEVEL__ 0
#define  __WSDEBUG_NONE__ 0
#define __WSDEBUG_ERROR__ -1
#define __WSDEBUG_DRIVER__ 9
#undef  __WSDEBUG__  

#ifdef  __WSDEBUG__ 
#define WSDEBUG_TPRINTF(...)  wsTextPrintf(__VA_ARGS__);
#define WSDEBUG_MPRINTF(...)  wsMonitorPrintf(__VA_ARGS__);
#define WSDEBUGCHK(level)  if (level<__DEBUG_LEVEL__) 
#else
#define WSDEBUG_TPRINTF(...)    
#define WSDEBUG_MPRINTF(...)     
#define WSDEBUGCHK(level)    
#endif

#endif //__DEBUGLOG__H__
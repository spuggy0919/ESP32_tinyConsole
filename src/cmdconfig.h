#ifndef __LANGSCONFIG_H__
#define __LANGSCONFIG_H__
/* when you have memory concerns please add into cmdconfig to enable/disable*/

// Language Config
#define TINYBASIC 
#include "TinyBasic.h" // if not include the pio will not get dependency for lib

// need to add below into lib_dep of platform.ini
// then define JERRYSCRIPT and  #include "JerryScript_tc.h"
	// lib_dep = https://github.com/spuggy0919/Arduino_Portenta_JerryScript.git  ; javascript 
#ifndef TINYBASIC
#define JERRYSCRIPT 
#include "JerryScript_tc.h"
#endif

/* wsTask manager */
#define _TICKER_TASK_

// device or APP config
#undef CMD_MQTT
#define CMD_DHT
#undef CMD_MESH
#define CMD_TIME

#include "command.h"
#endif //__LANGSCONFIG_H__
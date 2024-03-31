#ifndef __LANGSCONFIG_H__
#define __LANGSCONFIG_H__
/* when you have memory concerns please add into cmdconfig to enable/disable*/

// for Banner display and language package license notice
#define _LICENSE_BANNER_NOTICE_

// Language Config
// #define TINYBASIC /* 打開此為TINYBASIC 無定義視為 JERRYSCRIPT */
// #include "TinyBasic.h" // if not include the pio will not get dependency for lib

// need to add below into lib_dep of platform.ini
// then define JERRYSCRIPT and  #include "JerryScript_tc.h"
	// lib_dep = https://github.com/spuggy0919/Arduino_Portenta_JerryScript.git  ; javascript 
#ifndef TINYBASIC
#define _LANG_JERRYSCRIPT_ 
#include "JerryScript_tc.h"
#endif

#define _LICENSE_BANNER_NOTICE_
/* wsTask manager */
// #define FREERTOS_TIMERTASK // setTimeout setInterval clearInterval
// device or APP config
#define  CMD_FILE_EXT
#define CMD_MQTT
#define CMD_DHT
#undef  CMD_MESH
#define CMD_TIME

#include "command.h"
#endif //__LANGSCONFIG_H__
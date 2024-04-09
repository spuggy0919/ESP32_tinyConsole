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
	// lib_dep = https://github.com/spuggy0919/Arduino_Portenta_JerryScript.git  ; jerryscript 
#ifndef TINYBASIC
#define _LANG_JERRYSCRIPT_ 
#include "JerryScript_tc.h"
#undef _JS_EXAMPLE_
#define  _LIB_WIRE_    // ARDUINO WIRE support
#undef  _LIB_SPI_    // ARDUINO SPI support
#endif //ifndef TINYBASIC

#define _LICENSE_BANNER_NOTICE_
/* wsTask manager */
// #define FREERTOS_TIMERTASK // setTimeout setInterval clearInterval
// device or APP config

// direct running autorun for js use index.js 
// undef will use task background autorun, interpreter shell will life, but interference 
// the backgroup autorun task, still no idea to solve this
#define STARTUP_DIRECT_RUNAUTO 

// jerryscript c++ wrapper
#undef _CLASSOBJ_EXAMPLE_
// autogenerator wrapper test
#define _LIB_RECTANGLE_

// shell interpreter CMD disable for save prog space
#define  CMD_FILE_EXT
#define CMD_MQTT
#define CMD_DHT
#undef  CMD_MESH
#define CMD_TIME

#include "command.h"
#endif //__LANGSCONFIG_H__
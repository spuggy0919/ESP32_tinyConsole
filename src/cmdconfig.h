#ifndef __LANGSCONFIG_H__
#define __LANGSCONFIG_H__
/* when you have memory concerns please add into cmdconfig to enable/disable*/

// for Banner display and language package license notice, version 
#undef _LICENSE_BANNER_NOTICE_

// Language Config
#undef TINYBASIC /* 打開此為TINYBASIC 無定義視為 JERRYSCRIPT */
// #include "TinyBasic.h" // if not include the pio will not get dependency for lib

// need to add below into lib_dep of platform.ini
// then define JERRYSCRIPT and  #include "JerryScript_tc.h"
	// lib_dep = https://github.com/spuggy0919/Arduino_Portenta_JerryScript.git  ; jerryscript 
#ifndef TINYBASIC

// _LANG_JERRYSCRIPT_
#define _LANG_JERRYSCRIPT_ 
#ifdef _LANG_JERRYSCRIPT_
#include "JerryScript_tc.h"
// Below define is wrapper library, if need turnon, othwise for saving program size
#define _LIB_ESPCLASS_
#define _LIB_LIQUIDCRYSTAL_I2C_    // need ARDUINO WIRE support
#define _LIB_ADAFRUIT_SSD1306_ 
#undef  _LIB_SPI_     			   // ARDUINO SPI support
#define _LIB_TWOWIRE_			   // WIRE
// jerryscript c++ wrapper
#undef  _CLASSOBJ_EXAMPLE_ // same as RECtANGLE maunal 
// autogenerator wrapper test & CMD_example
#define _LIB_RECTANGLE_
#undef   _JS_EXAMPLE_      // JERRYSCRIPT API EXAMPLES


#endif //ifndef _LANG_JERRYSCRIPT_
#endif //ifndef TINYBASIC

/* wsTask manager */
// #define FREERTOS_TIMERTASK // setTimeout setInterval clearInterval
// device or APP config move to autorun 

// direct running autorun for js use index.js 
// undef will use task background autorun, interpreter shell will life, but interference 
// the backgroup autorun task, still no idea to solve this
#define STARTUP_DIRECT_RUNAUTO 




// shell interpreter CMD disable for save prog space
#define CMD_FILE_EXT
#define CMD_MQTT
#define CMD_WIRE
#define CMD_DHT
#undef  CMD_MESH
#define CMD_TIME
#define CMD_MPI    // for esp32 call mdnsqueryservices to find others nodes in LAN

#endif //__LANGSCONFIG_H__
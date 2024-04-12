/*
 * This file is part of ESP32_TinyConsole.
 *
 *  Copyright (c) 2024 spuggy0919
 *
 * ESP32_TinyConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESP32_TinyConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ESP32_TinyConsole.  If not, see 
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#ifndef _JSWRAP_TC_H_
#define _JSWRAP_TC_H_
#include "Arduino_Portenta_JerryScript.h"
#include "debuglog.h" 

#include "../../src/cmdconfig.h" 


#ifdef _LANG_JERRYSCRIPT_
#define JS_METHOD(f)                                         \
  jerry_value_t                                                             \
  js_##f (const jerry_call_info_t *call_info_p, /**< call information */    \
          const jerry_value_t args_p[], /**< function arguments */          \
          const jerry_length_t args_cnt) /**< number of function arguments */

/* sheel interpreter function */
int InterpreterArgvList(String *cmd, int* argc, char** argv[]);
int InterpreterExcute(String *cmd);

String checkExtension(const char *filename);
bool execScriptsFile(const char *path);
bool parserRunScriptsFile(const char *path);
int execShellCmd(const char *cmdline); // bug:maybe reentry for repl only

// print, setInterval, setTimeout, clearInterval,clearTimeout;
jerry_value_t register_module_class();

// print, setInterval, setTimeout, clearInterval,clearTimeout;
bool jerryxx_register_extra_api(void);
bool jerryxx_register_extra_api_free(void);
//arduino
void jerryxx_register_arduino_library();
//device
bool js_wsSerial_classobj_wraper();
//graphics
bool js_graphics_classobj_wraper();
//touch
bool js_touch_classobj_wraper();
//fsfilelib 
bool js_tcfilelib_classobj_wraper();
jerry_value_t register_fs_module();
//wifi
bool js_wifi_classobj_wraper(); //1

//esp32 
bool js_esp32_classobj_wraper();
//Wire
#ifdef  _LIB_ADAFRUIT_SSD1306_
bool js_sd1306_classobj_wrapper(); 
#endif //_LIB_ADAFRUIT_SSD1306_

#ifdef _LIB_LIQUIDCRYSTAL_I2C_
// LiquidCrystal_I2C.h
bool js_liquidlcd_classobj_wrapper();
#endif
// example c++ class obj wrap
#ifdef _CLASSOBJ_EXAMPLE_
bool js_cobj_classobj_wraper(); //1 a)modified func name and b) define in .h c) call by jswwrap_tc
#endif 
// autogen
#ifdef _LIB_RECTANGLE_
bool js_rectangle_classobj_wraper();
#endif
//mqtt
#ifdef CMD_MQTT
bool js_mqtt_classobj_wraper();
#endif

#ifdef CMD_DHT
bool js_dht_classobj_wraper();
#endif //CMD_DHT


void jerryxx_free_library();

#endif //_LANG_JERRYSCRIPT_

#endif //_JSWRAP_TC_H_

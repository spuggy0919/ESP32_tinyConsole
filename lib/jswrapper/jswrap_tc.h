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
#include "ESP32inc.h"

#include "../../src/cmdconfig.h"


#ifdef _LANG_JERRYSCRIPT_

// print, setInterval, setTimeout, clearInterval,clearTimeout;
jerry_value_t register_module_class();

// print, setInterval, setTimeout, clearInterval,clearTimeout;
bool jerryxx_register_extra_api(void);
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

//mqtt
#ifdef CMD_MQTT
bool js_mqtt_classobj_wraper();
#endif

#ifdef CMD_DHT
bool js_dht_classobj_wraper();
#endif //CMD_DHT

#endif //_LANG_JERRYSCRIPT_

#endif //_JSWRAP_TC_H_

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




#include "jswrap_tc.h"
void jerryxx_register_arduino_library(){
  /* Register the print function in the global object */
     jerryxx_register_extra_api(); 
     /* device */
     js_wsSerial_classobj_wraper();
     /*graphics*/
     js_graphics_classobj_wraper();
     /*touch*/
     js_touch_classobj_wraper();
     /*filelib File*/
     js_tcfilelib_classobj_wraper();

#ifdef CMD_DHT
     /*mqtt*/
     js_mqtt_classobj_wraper();
#endif
     /* sensor */
#ifdef CMD_DHT
     js_dht_classobj_wraper();
#endif
    /* library */

}



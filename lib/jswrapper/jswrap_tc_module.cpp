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
String checkExtension(const char *filename){
        String fname = String(filename);
        String dot =".";
        if (fname.indexOf(dot) == -1) {
            fname+=".js";
        }
        return fname;
}
int InterpreterExcute(String *cmd);
int execShellCmd(const char *cmdline){
    String cmdString = String(cmdline);
    InterpreterExcute(&cmdString); //bug possible reentry 
    return true;
}

bool execScriptsFile(const char *path){
    String fname_ext = checkExtension(path);
    jerry_size_t result = jerryx_source_exec_script ((const char *)fname_ext.c_str());
        if (jerry_value_is_error(result)) {
            JERRYX_ERROR_MSG("exec: %d\n", jerry_value_is_error(result));
            return false;
        }
    jerry_value_free(result);
    return true;
}

bool parserRunScriptsFile(const char *path){
    String fname_ext = checkExtension(path);
    jerry_size_t length;
    jerry_value_t print_result;
    jerry_value_t script;

    jerry_char_t* scripts = jerry_port_source_read ((const char *)fname_ext.c_str(),&length);
        if (scripts == NULL) {
            // read fail
            return false;
        }
    jerry_size_t result =  jerry_parse (scripts, length, NULL);
    jerry_port_source_free(scripts);
    if (jerry_value_is_exception (result)) {
        JERRYX_ERROR_MSG("parser: %d\n", jerry_value_is_exception(result));
        return false;
    }
    script = result;
    result = jerry_run (script);
    jerry_value_free (script);
    if (jerry_value_is_exception (result))
    {
        jerryx_print_unhandled_exception (result);
    }
    print_result = jerryx_print_value (result);
    jerry_value_free (result);
    result = print_result;
    jerry_value_free(result);
    return true;
}

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
     /*wifi obj*/
     js_wifi_classobj_wraper(); //1
     /*esp32 info obj*/
     js_esp32_classobj_wraper();

#ifdef _LIB_LIQUIDCRYSTAL_I2C_
// LiquidCrystal_I2C.h
      js_liquidlcd_classobj_wrapper(); // register 
#endif

#ifdef  _LIB_ADAFRUIT_SSD1306_
      js_sd1306_classobj_wrapper(); 
#endif //_LIB_ADAFRUIT_SSD1306_

#ifdef _CLASSOBJ_EXAMPLE_
     js_cobj_classobj_wraper(); //1 a)modified func name and b) define in .h c) call by jswwrap_tc
#endif 
#ifdef _LIB_RECTANGLE_
     js_rectangle_classobj_wraper();
#endif
#ifdef CMD_DHT
     /*mqtt*/
     js_mqtt_classobj_wraper();
#endif
     /* sensor */
#ifdef CMD_DHT
     js_dht_classobj_wraper();
#endif
    /* library */

    /* module*/
    if (!execScriptsFile("/js/modules/module.js")) return; // require('module');

}


void jerryxx_free_library(){
    /* call necessay free before jerry_cleanup */
    jerryxx_register_extra_api_free();

}

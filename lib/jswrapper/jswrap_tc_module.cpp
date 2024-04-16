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
// arg mapping 
/**
 * Define transformer for optional argument.
 */
jerry_value_t
jerryx_arg_transform_arraybuffer (jerryx_arg_js_iterator_t *js_arg_iter_p, /**< available JS args */
                              const jerryx_arg_t *c_arg_p); /**< the native arg */
#define JERRYX_ARG_TRANSFORM_OPTIONAL(type)                                                      \
  jerry_value_t jerryx_arg_transform_##type##_optional (jerryx_arg_js_iterator_t *js_arg_iter_p, \
                                                        const jerryx_arg_t *c_arg_p)             \
  {                                                                                              \
    return jerryx_arg_transform_optional (js_arg_iter_p, c_arg_p, jerryx_arg_transform_##type);  \
  }
JERRYX_ARG_TRANSFORM_OPTIONAL (arraybuffer)
#undef JERRYX_ARG_TRANSFORM_OPTIONAL                                                    \


jerry_value_t
jerryx_arg_transform_arraybuffer (jerryx_arg_js_iterator_t *js_arg_iter_p, /**< available JS args */
                              const jerryx_arg_t *c_arg_p) /**< the native arg */
{
  jerry_value_t js_arg = jerryx_arg_js_iterator_pop (js_arg_iter_p);
  uint8_t** dest = (uint8_t**)c_arg_p->dest;
  uint8_t* buffer;
  if (jerry_value_is_arraybuffer (js_arg)) {
    buffer = jerry_arraybuffer_data (js_arg);
    // WSDEBUG_TPRINTF("[transform] ArrayBuffer is %x\n",buffer);
  }else if (jerry_value_is_typedarray (js_arg)) { // typedarray
    size_t tlength = jerry_typedarray_length(js_arg);
    size_t ttype = jerry_typedarray_type(js_arg);
    jerry_length_t byteOffset=0;
    jerry_length_t byteLength=0;
    jerry_value_t abuffer = jerry_typedarray_buffer(js_arg,&byteOffset,&byteLength);
    buffer = jerry_arraybuffer_data (abuffer);
    // WSDEBUG_TPRINTF("[transform] typedarrayis %x araybuf is %x\n",abuffer,buffer);
    jerry_value_free(abuffer);
  }
  *dest = buffer;
  return jerry_undefined ();
} /* jerryx_arg_transform_arraybuffer */

// arg mapping 
jerry_value_t
jerryx_arg_transform_arraybuffer_strict (jerryx_arg_js_iterator_t *js_arg_iter_p, /**< available JS args */
                              const jerryx_arg_t *c_arg_p) /**< the native arg */
{
  jerry_value_t js_arg = jerryx_arg_js_iterator_pop (js_arg_iter_p);
  if (!jerry_value_is_arraybuffer (js_arg)&&!jerry_value_is_typedarray (js_arg))
  {
    return jerry_throw_sz (JERRY_ERROR_TYPE, "It is not a arraybuffer and not typedarray.");
  }
  uint8_t** dest = (uint8_t**) c_arg_p->dest;
  uint8_t* buffer;
  if (jerry_value_is_arraybuffer (js_arg)) {
    buffer = jerry_arraybuffer_data (js_arg);
  }else if (jerry_value_is_typedarray (js_arg)) { // typedarray
    size_t tlength = jerry_typedarray_length(js_arg);
    size_t ttype = jerry_typedarray_type(js_arg);
    jerry_length_t byteOffset=0;
    jerry_length_t byteLength=0;
    jerry_value_t abuffer = jerry_typedarray_buffer(js_arg,&byteOffset,&byteLength);
    buffer = jerry_arraybuffer_data (abuffer);
    jerry_value_free(abuffer);
  }
  *dest = buffer;
  return jerry_undefined ();
} /* jerryx_arg_transform_arraybuffer */





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

#ifdef _LIB_ESPCLASS_
     /*esp32 info obj*/
// ESP.h
     js_espclass_classobj_wrapper();
#endif
#ifdef _LIB_LIQUIDCRYSTAL_I2C_
// LiquidCrystal_I2C.h
      js_liquidlcd_classobj_wrapper(); // register 
#endif

#ifdef  _LIB_ADAFRUIT_SSD1306_
      js_sd1306_classobj_wrapper(); 
#endif //_LIB_ADAFRUIT_SSD1306_
#ifdef _LIB_TWOWIRE_
      js_twowire_classobj_wrapper();
#endif //_LIB_TWOWIRE_
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

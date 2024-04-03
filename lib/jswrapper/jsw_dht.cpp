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

#ifdef _LANG_JERRYSCRIPT_
#ifdef CMD_DHT
#include "DHTesp.h"
DHTesp dht;

// methods define 
JERRYXX_DECLARE_FUNCTION(dht_init){
  JERRYX_UNUSED(call_info_p);
  uint32_t pin = 0;

  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_uint32(&pin, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };

  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
   dht.setup((uint8_t)pin,DHTesp::DHT22);
   // Wrap the C++ object in a JerryScript object
  return jerry_undefined();

} /*js_dht_init*/
JERRYXX_DECLARE_FUNCTION(dht_get_status){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

  return jerry_number(dht.getStatus());
} /*js_dht_get_status*/


JERRYXX_DECLARE_FUNCTION(dht_get_temperature){

  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

  return jerry_number(dht.getTemperature());
} /*js_dht_get_tempature*/

JERRYXX_DECLARE_FUNCTION(dht_get_humidity){

  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

  return jerry_number(dht.getHumidity());
} /*js_dht_get_humidity*/


// class object wrapper
bool js_dht_classobj_wraper(){
  bool ret = false;
          // Register initialization function
 jerryx_property_entry methods[] =
  {
    JERRYX_PROPERTY_STRING_SZ ("id", "dht"),
    JERRYX_PROPERTY_FUNCTION ("setup", js_dht_init),
    JERRYX_PROPERTY_FUNCTION ("getStatus", js_dht_get_status),
    JERRYX_PROPERTY_FUNCTION ("getTemperature", js_dht_get_temperature),
    JERRYX_PROPERTY_FUNCTION ("getHumidity", js_dht_get_humidity),
    JERRYX_PROPERTY_LIST_END(),
  };


  jerry_value_t object = jerry_object();
  jerryx_register_result reg = jerryx_set_properties (object, methods);

  if (jerry_value_is_exception (reg.result))
  {
    wsTextPrintf ("Only registered %d properties\r\n", reg.registered);
    /* clean up not registered property values */
    jerryx_release_property_entry (methods, reg);
    jerry_value_free (reg.result);

    /* clean up the error */
  }
  /* Set the native function as a property of the empty JS object */

  // wrap to dht 
  jerry_value_t globalObject = jerry_current_realm ();
  jerry_value_t prop_name = jerry_string_sz((const char*)"dht");
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}

bool js_dht_classobj_wraper1(){
  /* Create an empty JS object */
  jerry_value_t object = jerry_object ();
  /* Set the native function as a property of the empty JS object */
  jerry_value_t prop_name0 = jerry_string_sz ("setup");
  jerry_value_t func_obj0 = jerry_function_external (js_dht_init);
  jerry_value_free (jerry_object_set (object, prop_name0, func_obj0));
  jerry_value_free (prop_name0);
  jerry_value_free (func_obj0);

  // prop 1
  jerry_value_t prop_name1 = jerry_string_sz ("getStatus");
  jerry_value_t func_obj1 = jerry_function_external (js_dht_get_status);
  jerry_value_free (jerry_object_set (object, prop_name1, func_obj1));
  jerry_value_free (prop_name1);
  jerry_value_free (func_obj1);
  // prop 2
  jerry_value_t prop_name2 = jerry_string_sz ("getTemperature");
  jerry_value_t func_obj2 = jerry_function_external (js_dht_get_temperature);
  jerry_value_free (jerry_object_set (object, prop_name2, func_obj2));
  jerry_value_free (prop_name2);
  jerry_value_free (func_obj2);
  // prop 3
  jerry_value_t prop_name3 = jerry_string_sz ("getHumidity");
  jerry_value_t func_obj3 = jerry_function_external (js_dht_get_humidity);
  jerry_value_free (jerry_object_set (object, prop_name3, func_obj3));
  jerry_value_free (prop_name3);
  jerry_value_free (func_obj3);  
  // wrap to dht 
  jerry_value_t globalObject = jerry_current_realm ();
  jerry_value_t prop_name = jerry_string_sz((const char*)"dht");
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);
    return true;
}

#endif
#endif
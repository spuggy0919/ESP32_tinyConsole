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

/**/

typedef  jerry_value_t (*MODULE_INIT)(); /**< number of function arguments */

typedef struct jerry_module {
    const char*      name;
    MODULE_INIT     module_register;
    jerry_value_t    moduleobj;
}JERRY_MODULE;

// #define BUILDIN_MODULES 10
// jerry_value_t exports[BUILDIN_MODULES];

JERRY_MODULE buildin_module[] = {
   { "fs",register_fs_module, 0}
    // "sio",register_sio_module,
};

JERRYXX_DECLARE_FUNCTION(modules_init){ //1
  JERRYX_UNUSED(call_info_p);
  char      mname[32]; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(mname, sizeof(mname), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
   return jerry_undefined();
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

   for(int i;i<sizeof(buildin_module);i++){
        if(!strcmp(mname,buildin_module[i].name)){
            if (buildin_module[i].moduleobj == 0) {
                    buildin_module[i].moduleobj = buildin_module[i].module_register();
            } else {
                    return buildin_module[i].moduleobj;
            }
        }
   }
   return jerry_undefined();
} /*js_modules_init*/ //6

// class object wrapper
bool js_module_classobj_wraper(){
  bool ret = false;
          // Register initialization function
  jerryx_property_entry methods[] ={
    // JERRYX_PROPERTY_FUNCTION ("name", js_fs_classname),
    JERRYX_PROPERTY_FUNCTION ("init", js_modules_init),
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
    return false;
    /* clean up the error */
  }
  /* Set the native function as a property of the empty JS object */

  // wrap to dht 
  jerry_value_t globalObject = jerry_current_realm ();
  jerry_value_t prop_name = jerry_string_sz((const char*)"Module");
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}

jerry_value_t register_module_class(){
          // Register initialization function
  jerryx_property_entry methods[] ={
    JERRYX_PROPERTY_FUNCTION ("MODULE", js_modules_init),
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
    return false;
    /* clean up the error */
  }
  /* Set the native function as a property of the empty JS object */
  // wrap to fs module

  jerry_value_t exports_object = jerry_object();
  jerry_value_t module_name = jerry_string_sz((const char*)"fs");
  jerry_value_free (jerry_object_set (exports_object, module_name, object));


  jerry_value_free (object);
  jerry_value_free (module_name);
  jerry_value_free (exports_object);

  return exports_object;
}

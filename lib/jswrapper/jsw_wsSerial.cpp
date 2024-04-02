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
// class WebSocketSerial {  // no inherits from arduino print : sccording virtual public Print
// public:
//     WebSocketSerial();
//     ~WebSocketSerial();
//     const char*classname;
JERRYXX_DECLARE_FUNCTION(wsSerial_classname){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_string_sz(wsSerial.classname);
} /*js_wsSerial_classname*/
//     bool begin();
JERRYXX_DECLARE_FUNCTION(wsSerial_begin){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_boolean(wsSerial.begin());
} /*js_wsSerial_begin*/
//     bool flush();
JERRYXX_DECLARE_FUNCTION(wsSerial_flush){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_boolean(wsSerial.flush());
} /*js_wsSerial_flush*/
//     size_t available();
JERRYXX_DECLARE_FUNCTION(wsSerial_available){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_number(wsSerial.available());
} /*js_wsSerial_available*/
//     size_t availableForWrite();
JERRYXX_DECLARE_FUNCTION(wsSerial_availableForWrite){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_number(wsSerial.availableForWrite());
} /*js_wsSerial_availableForWrite*/
//     char read();
JERRYXX_DECLARE_FUNCTION(wsSerial_read){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  char buf[2];
  buf[0]=wsSerial.read();
  buf[1]=0;
  // Serial.printf("[jswrap]:read[%2x]%c\n",buf[0],buf[0]);  
  return jerry_string_sz(buf);
} /*js_wsSerial_read*/
//     char getChar();
JERRYXX_DECLARE_FUNCTION(wsSerial_getChar){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  char buf[2];
  buf[0]=wsSerial.getChar();
  buf[1]=0;
  // Serial.printf("[jswrap]:getChar[%2x]%c\n",buf[0],buf[0]);  
  return jerry_string_sz(buf);
  } /*js_wsSerial_getChar*/
    // bool readLine(char *buf,int *len);
JERRYXX_DECLARE_FUNCTION(wsSerial_readLine){
  JERRYX_UNUSED(call_info_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  char buf[128];
  uint32_t  len=128;
  // const jerryx_arg_t mapping[] =
  //     {
  //         jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
  //         jerryx_arg_uint32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
  //     };

  // const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  // if (jerry_value_is_exception(rv)){
  //   return rv;
  // }
  // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");
  bool ret = false; 
  wsSerial.readLine(buf,(int*)&len); 
  // result buffer set back to argv TODO 
  return jerry_string_sz(buf);
} /*js_wsSerial_readLine*/

//     bool escape();
JERRYXX_DECLARE_FUNCTION(wsSerial_escape){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  bool b=wsSerial.escape();
  // Serial.printf("[jswrap]:escape[%2x]%c\n",b,b);
  return jerry_boolean(b);
} /*js_wsSerial_escape*/

//     size_t write(uint8_t c);
// JERRYXX_DECLARE_FUNCTION(wsSerial_write){
//   JERRYX_UNUSED(call_info_p);
//   uint32_t c;
//   const jerryx_arg_t mapping[] =
//       {
//           jerryx_arg_uint32(&c, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
//       };

//   const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
//   if (jerry_value_is_exception(rv)){
//     return rv;
//   }
//   return jerry_number(wsSerial.write((uint8_t)c));
// } /*js_wsSerial_write*/
//     size_t write(const char* buf,int len);
JERRYXX_DECLARE_FUNCTION(wsSerial_writestring){
  JERRYX_UNUSED(call_info_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");
  char buf[128];
  uint32_t  len=128;  
  const jerryx_arg_t mapping[] =
      {
      jerryx_arg_utf8_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      // jerryx_arg_uint32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };

  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }

  return jerry_number(wsSerial.write(buf,sizeof(buf)-1));
} /*js_wsSerial_write*/
JERRYXX_DECLARE_FUNCTION(wsSerial_printf){
  JERRYX_UNUSED(call_info_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");
  char buf[128];
  uint32_t  len=128;  
  const jerryx_arg_t mapping[] =
      {
      jerryx_arg_utf8_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      // jerryx_arg_uint32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };

  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  wsTextPrintf(buf);
  return jerry_undefined();
} /*js_wsSerial_printf*/

//     char peek();
JERRYXX_DECLARE_FUNCTION(wsSerial_peek){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  char buf[2];
  buf[0]=wsSerial.peek();
  buf[1]=0;
  Serial.printf("[jswrap]:peek[%2x]%c\n",buf[0],buf[0]);  
  return jerry_string_sz(buf);
} /*js_wsSerial_peek*/
//     size_t push(char c);
JERRYXX_DECLARE_FUNCTION(wsSerial_push){
  JERRYX_UNUSED(call_info_p);
  uint32_t c;
  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_uint32(&c, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };

  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }  
  return jerry_char_t(wsSerial.push((uint8_t)c));
} /*js_wsSerial_push*/
//     size_t push(const char *buf, int len); 
//     char pop();
JERRYXX_DECLARE_FUNCTION(wsSerial_pop){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_char_t(wsSerial.pop());
} /*js_wsSerial_pop*/
//     bool popLine(char *buf, int *len); 

// class object wrapper
bool js_wsSerial_classobj_wraper(){
  bool ret = false;
          // Register initialization function
  jerryx_property_entry methods[] ={
    JERRYX_PROPERTY_FUNCTION ("name", js_wsSerial_classname),
    JERRYX_PROPERTY_FUNCTION ("begin", js_wsSerial_begin),
    JERRYX_PROPERTY_FUNCTION ("flush", js_wsSerial_flush),
    JERRYX_PROPERTY_FUNCTION ("available", js_wsSerial_available),
    JERRYX_PROPERTY_FUNCTION ("availableForWrite", js_wsSerial_availableForWrite),
    JERRYX_PROPERTY_FUNCTION ("read", js_wsSerial_read),
    JERRYX_PROPERTY_FUNCTION ("readline", js_wsSerial_readLine),
    JERRYX_PROPERTY_FUNCTION ("getchar", js_wsSerial_getChar),
    JERRYX_PROPERTY_FUNCTION ("escape", js_wsSerial_escape),
    // JERRYX_PROPERTY_FUNCTION ("writebyte", js_wsSerial_write),
    JERRYX_PROPERTY_FUNCTION ("writestring", js_wsSerial_writestring),
    JERRYX_PROPERTY_FUNCTION ("print", js_wsSerial_printf),
    JERRYX_PROPERTY_FUNCTION ("peek", js_wsSerial_peek),
    // JERRYX_PROPERTY_FUNCTION ("push", js_wsSerial_push),
    // JERRYX_PROPERTY_FUNCTION ("pop", js_wsSerial_pop),
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
  jerry_value_t prop_name = jerry_string_sz((const char*)"wsSerial");
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}
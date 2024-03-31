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
#include "ESP32inc.h"

    // size_t available();
    // void push(String msg);
    // void push(int event, int x, int y);
    // bool pop(int &event, int &x, int &y);

static struct TouchEvent{
    int event;
    int x; 
    int y;
}tevent;
// methods define 
    // bool flush();
JERRYXX_DECLARE_FUNCTION(touch_begin){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4
  tevent.event=0;
  tevent.x=0;
  tevent.y=0;
  gTouchQueue.flush();
  return jerry_boolean(gTouchQueue.flush());
} /*js_touch_begin*/ //6
JERRYXX_DECLARE_FUNCTION(touch_attach){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4

  return jerry_boolean(WebWSConnect());
} /*js_touch_attach*/ //6


// bool pop(int &event, int &x, int &y);
// wrap as 
// class touch{
//     event: int;
//     x: int;
//     y: int;
// } touchobj;
//   touchObj = {event, x, y}
//  return touchObj


JERRYXX_DECLARE_FUNCTION(touch_getevent){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4
  bool ret = gTouchQueue.pop(tevent.event,tevent.x,tevent.y);

 if (!ret) return  jerry_number(0);
  return jerry_number(tevent.event);
} /*js_touch_getevent*/ //6

JERRYXX_DECLARE_FUNCTION(touch_getx){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4

  return jerry_number(tevent.x);
} /*js_touch_getx*/ //6

JERRYXX_DECLARE_FUNCTION(touch_gety){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4

  return jerry_number(tevent.y);
} /*js_touch_gety*/ //6

JERRYXX_DECLARE_FUNCTION(touch_classname){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4

  return jerry_string_sz("Touch");
} /*js_touch_classname*/ //6

// class object wrapper
bool js_touch_classobj_wraper(){  //1 a)modified func name and b) define in .h c) call by jswwrap_tc
  bool ret = false;
  // Register initialization function
  jerryx_property_entry methods[] ={ //2 methods
    JERRYX_PROPERTY_FUNCTION ("name", js_touch_classname),
    JERRYX_PROPERTY_FUNCTION ("begin", js_touch_begin),
    JERRYX_PROPERTY_FUNCTION ("attach", js_touch_attach),
    JERRYX_PROPERTY_FUNCTION ("event", js_touch_getevent),
    JERRYX_PROPERTY_FUNCTION ("x", js_touch_getx),
    JERRYX_PROPERTY_FUNCTION ("y", js_touch_gety),
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
  jerry_value_t prop_name = jerry_string_sz((const char*)"Touch"); // 3 Class name
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}

#endif

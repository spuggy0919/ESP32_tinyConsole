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
#include "ESP32inc.h"

#ifdef _LANG_JERRYSCRIPT_
JERRYXX_DECLARE_FUNCTION(esp_get_heapsize){

  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

  return jerry_number(ESP.getHeapSize());
} /*js_esp_get_heapsize*/
JERRYXX_DECLARE_FUNCTION(esp_get_freeheap){

  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

  return jerry_number(ESP.getFreeHeap());
} /*js_esp_get_freeheap*/
JERRYXX_DECLARE_FUNCTION(esp_get_minfreeheap){

  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

  return jerry_number(ESP.getMinFreeHeap());
} /*js_esp_get_minfreeheap*/
JERRYXX_DECLARE_FUNCTION(esp_get_maxallocheap){

  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

  return jerry_number(ESP.getMaxAllocHeap());
} /*js_esp_get_maxallocheap*/
// class object wrapper
bool js_esp32_classobj_wraper(){
  bool ret = false;
          // Register initialization function
  jerryx_property_entry methods[] ={
    JERRYX_PROPERTY_STRING_SZ ("id", "esp32"),
    JERRYX_PROPERTY_STRING_SZ ("ChipIModel", ESP.getChipModel()),
    JERRYX_PROPERTY_NUMBER ("ClockFreq", ESP.getCpuFreqMHz()),

    JERRYX_PROPERTY_NUMBER ("FlashSize", ESP.getFlashChipSize()),
    JERRYX_PROPERTY_NUMBER ("FlashChipSpeed", ESP.getFlashChipSpeed()),
    JERRYX_PROPERTY_NUMBER ("FlashChipMode", ESP.getFlashChipMode()),

    JERRYX_PROPERTY_NUMBER ("PsramSize", ESP.getPsramSize()),
    JERRYX_PROPERTY_NUMBER ("FreePsram", ESP.getFreePsram()),
    JERRYX_PROPERTY_NUMBER ("MinFreePsram", ESP.getMinFreePsram()),
    JERRYX_PROPERTY_NUMBER ("MaxAllocPsram", ESP.getMaxAllocPsram()),

    JERRYX_PROPERTY_FUNCTION ("HeapSize", js_esp_get_heapsize),
    JERRYX_PROPERTY_FUNCTION ("FreeHeap", js_esp_get_freeheap),
    JERRYX_PROPERTY_FUNCTION ("MinFreeHeap", js_esp_get_minfreeheap),
    JERRYX_PROPERTY_FUNCTION ("MaxAllocHeap", js_esp_get_maxallocheap),

    JERRYX_PROPERTY_NUMBER ("SketchSize", ESP.getSketchSize()),
    JERRYX_PROPERTY_NUMBER ("FreeSketchSpace", ESP.getFreeSketchSpace()),
    JERRYX_PROPERTY_STRING_SZ ("SketchMD5", ESP.getSketchMD5().c_str()),
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
  jerry_value_t prop_name = jerry_string_sz((const char*)"esp32");
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}

#endif
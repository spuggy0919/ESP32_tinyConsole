
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
#include "wifiinit.h"
//   1. WiFi.mode(WIFI_STA);
JERRYXX_DECLARE_FUNCTION(wifi_mode){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   mode; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&mode, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

   // Set WiFi mode based on the integer value
  switch (mode) {
    case 1:
      WiFi.mode(WIFI_STA); // Set WiFi mode to Station mode (client)
      break;
    case 2:
      WiFi.mode(WIFI_AP); // Set WiFi mode to Access Point mode
      break;
    // Add more cases for other WiFi modes if needed
    default:
      break;
  } //5
  return jerry_undefined();
} /*js_wifi_mode*/ //6

//   2. local_IP.fromString(ip.c_str());
//     local_Gateway.fromString(gateway.c_str());
//     if (!WiFi.config(local_IP, local_Gateway, subnet)){
// DHCP config no need
JERRYXX_DECLARE_FUNCTION(wifi_config){ //1
  JERRYX_UNUSED(call_info_p);
  char    local_IP[20]; //2
  char    local_gateway[20]; //2
  char    local_subnet[20]; //2
  char    local_dns1[20]; //2
  char    local_dns2[20]; //2
  const jerryx_arg_t mapping[] = //3
      {
      jerryx_arg_utf8_string(local_IP, sizeof(local_IP), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      jerryx_arg_utf8_string(local_gateway, sizeof(local_gateway), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      jerryx_arg_utf8_string(local_subnet, sizeof(local_subnet), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      jerryx_arg_utf8_string(local_dns1, sizeof(local_dns1), JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      jerryx_arg_utf8_string(local_dns2, sizeof(local_dns2), JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt < 3 , "Wrong arguments count");  //4
    IPAddress localIP; localIP.fromString(local_IP);
    IPAddress gw; gw.fromString(local_gateway);
    IPAddress subnet; subnet.fromString(local_subnet);
    IPAddress dns1;
    IPAddress dns2;
    bool ret=false;
    if (args_cnt > 3) dns1.fromString(local_dns1);
    if (args_cnt > 4) dns2.fromString(local_dns2);
    if (args_cnt == 3) ret=WiFi.config(localIP, gw, subnet);
    if (args_cnt == 4) ret=WiFi.config(localIP, gw, subnet,dns1);
    if (args_cnt == 5) ret=WiFi.config(localIP, gw, subnet,dns1,dns2);
  return jerry_boolean(ret);
} /*js_wifi_config*/ //6
//   3. WiFi.begin(ssid.c_str(), pass.c_str());
JERRYXX_DECLARE_FUNCTION(wifi_begin){ //1
  JERRYX_UNUSED(call_info_p);
  char    ssid[32]; //2
  char    password[32]; //2
  const jerryx_arg_t mapping[] = //3
      {
      jerryx_arg_utf8_string(ssid, sizeof(ssid), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      jerryx_arg_utf8_string(password, sizeof(password), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt !=2 , "Wrong arguments count");  //4
  
  return jerry_number(WiFi.begin(ssid,password));
} /*js_wifi_begin*/ //6
//   4. while(WiFi.status() != WL_CONNECTED) {
JERRYXX_DECLARE_FUNCTION(wifi_status){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_number(WiFi.status());
} /*js_wifi_status*/ //6

// String WifiLocalIP();
JERRYXX_DECLARE_FUNCTION(wifi_localip){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  
  return jerry_string_sz((WiFi.localIP().toString()).c_str());
} /*js_wifi_localip*/
// String WifiSSID();
JERRYXX_DECLARE_FUNCTION(wifi_ssid){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  
  return jerry_string_sz(WiFi.SSID().c_str());
} /*js_wifi_ssid*/
// long WifiRSSI();
JERRYXX_DECLARE_FUNCTION(wifi_rssi){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  
  return jerry_number(WiFi.RSSI());
} /*js_wifi_rssi*/
// int WifiMode();
JERRYXX_DECLARE_FUNCTION(wifi_getmode){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  
  return jerry_number(WiFi.getMode());
} /*js_wifi_getmode*/

JERRYXX_DECLARE_FUNCTION(wifi_classname){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_string_sz("wifi");
} /*js_wifi_classname*/

// class object wrapper
bool js_wifi_classobj_wraper(){ //1
  bool ret = false;
          // Register initialization function
  jerryx_property_entry methods[] ={ //2
    JERRYX_PROPERTY_STRING_SZ ("id", "wifi"),
    // JERRYX_PROPERTY_FUNCTION ("name", js_wifi_classname),
    JERRYX_PROPERTY_FUNCTION ("mode", js_wifi_mode),
    JERRYX_PROPERTY_FUNCTION ("config", js_wifi_config),
    JERRYX_PROPERTY_FUNCTION ("begin", js_wifi_begin),
    JERRYX_PROPERTY_FUNCTION ("status", js_wifi_status),
    JERRYX_PROPERTY_FUNCTION ("localIP", js_wifi_localip),
    JERRYX_PROPERTY_FUNCTION ("ssid", js_wifi_ssid),
    JERRYX_PROPERTY_FUNCTION ("rssi", js_wifi_rssi),
    JERRYX_PROPERTY_FUNCTION ("getmode", js_wifi_getmode),
    JERRYX_PROPERTY_NUMBER ("WIFI_STA", 1),
    JERRYX_PROPERTY_NUMBER ("WIFI_AP", 2), 
    JERRYX_PROPERTY_NUMBER ("WL_CONNECTED", 3), 

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
  jerry_value_t prop_name = jerry_string_sz((const char*)"wifi"); //3
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}
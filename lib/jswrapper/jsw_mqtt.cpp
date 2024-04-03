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
#ifdef CMD_MQTT
#include "PubSubClient.h"
extern WiFiClient espClient; //in cmd_mqtt
extern PubSubClient client; //in cmd_mqtt

// 1.client.setServer(_mqtt_server, 1883);
JERRYXX_DECLARE_FUNCTION(mqttclient_setserver){ //1
  JERRYX_UNUSED(call_info_p);
  char serverip_str[20]; //2
  uint16_t mqttport=1883;
  const jerryx_arg_t mapping[] = //3
      {
    jerryx_arg_string (serverip_str, sizeof (serverip_str), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    jerryx_arg_uint16(&mqttport, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX((args_cnt != 1 && args_cnt != 2 ) , "Wrong arguments count");  //4
   Serial.printf("[jsw_mqtt]:%s,%d\n",serverip_str,mqttport);
   IPAddress _mqtt_serverip(18,198,222,5);
   String ipstr = String(serverip_str);
   if (ipstr != "") {
      _mqtt_serverip.fromString(ipstr);
      client.setServer(_mqtt_serverip,mqttport); //5
   }
  return jerry_undefined();
} /*js_mqttclient_setserver*/ //6

static jerry_value_t jerry_mqtt_callback_fn = 0; //2
void mqtt_test(char* topic, byte* payload, unsigned int length) {
  wsTextPrintf("Message arrived [");
  wsTextPrintf(topic);
  wsTextPrintf("] ");
  for (int i = 0; i < length; i++) {
    wsTextPrintf("%c",(char)payload[i]);
  }
  wsTextPrintf("\n");

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    mqtt_test(topic,payload,length);
    return;

    // *setup argp argc
  jerry_value_t args[] = {
    jerry_string_sz (topic),
    jerry_string_sz ((char *)payload), // to do payload maybe binary shouldbe bytearray
    jerry_number (length),
  };
    jerry_size_t argc = sizeof (args) / sizeof (args[0]);
    jerry_value_t callback_fn_copy = jerry_value_copy (jerry_mqtt_callback_fn);
    jerry_value_t global_obj_val = jerry_current_realm ();
    jerry_value_t result_val = jerry_call (callback_fn_copy, global_obj_val, args, argc);
    jerry_value_free (result_val);
    jerry_value_free (global_obj_val);
    jerry_value_free (callback_fn_copy);            // Perform the periodic operation here
    return;
}

// 2.client.setCallback(callback);
JERRYXX_DECLARE_FUNCTION(mqttclient_setcallback){ //1
  JERRYX_UNUSED(call_info_p);
//   jerry_value_t callback_fn = 0; //2

  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_function(&jerry_mqtt_callback_fn, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX((args_cnt != 1) , "Wrong arguments count");  //4
   Serial.printf("[jsw_mqtt]:setcallback\n");

   client.setCallback(mqtt_callback); //5
  return jerry_undefined();
} /*js_mqttclient_setcallback*/ //6

// 3.client.connect(client_id);
JERRYXX_DECLARE_FUNCTION(mqttclient_connect){ //1
  JERRYX_UNUSED(call_info_p);
  char      client_id[64];

  const jerryx_arg_t mapping[] = //3
      {
           jerryx_arg_string (client_id, sizeof (client_id), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX((args_cnt != 1) , "Wrong arguments count");  //4

   Serial.printf("[jsw_mqtt]:connect(%s)\n",client_id);
   //5
  return jerry_boolean(client.connect(client_id)); 
} /*js_mqttclient_connect*/ //6

// 4.client.connected()
JERRYXX_DECLARE_FUNCTION(mqttclient_connected){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4
  bool ret = client.connected();
   Serial.printf("[jsw_mqtt]:connected(%d)\n",ret);
  return jerry_boolean(ret); 
} /*js_mqttclient_connected*/ //6

// 5.client.loop(); // may be not
JERRYXX_DECLARE_FUNCTION(mqttclient_loop){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4

     Serial.printf("[jsw_mqtt]:loop\n");
 //5
  return jerry_boolean(client.loop()); 
} /*js_mqttclient_loop*/ //6

// not necessary 2.void reconnect() 
//       void callback(char* topic, byte* payload, unsigned int length) 
//       wsTextPrintf("Message arrived callback [");
// 6.client.State;
JERRYXX_DECLARE_FUNCTION(mqttclient_state){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4
    int state = client.state();
         Serial.printf("[jsw_mqtt]:state(%d)\n",state);
   //5
  return jerry_number(state); 
} /*js_mqttclient_state*/ //6

// 7.client.publish("outTopic", "hello world");
JERRYXX_DECLARE_FUNCTION(mqttclient_publish){ //1
  JERRYX_UNUSED(call_info_p);
  char      topics[128];
  char      payload[128];

  const jerryx_arg_t mapping[] = //3
      {
           jerryx_arg_string (topics, sizeof (topics), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
           jerryx_arg_string (payload, sizeof (payload), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX((args_cnt != 2) , "Wrong arguments count");  //4
   //5
            Serial.printf("[jsw_mqtt]:public(%s,%s)\n",topics,payload);

  return jerry_number(client.publish(topics,payload)); 
} /*js_mqttclient_publish*/ //6

// 8.client.subscribe("inTopic");
JERRYXX_DECLARE_FUNCTION(mqttclient_subscribe){ //1
  JERRYX_UNUSED(call_info_p);
  char      topics[128];

  const jerryx_arg_t mapping[] = //3
      {
           jerryx_arg_string (topics, sizeof (topics), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX((args_cnt != 1) , "Wrong arguments count");  //4
   //5
  Serial.printf("[jsw_mqtt]:subscribe(%s)\n",topics);

  return jerry_number(client.subscribe(topics)); 
} /*js_mqttclient_subscribe*/ //6



JERRYXX_DECLARE_FUNCTION(mqtt_classname){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4

  return jerry_string_sz("MqttClient");
} /*js_mqtt_classname*/ //6


// class object wrapper
bool js_mqtt_classobj_wraper(){  //1 a)modified func name and b) define in .h c) call by jswwrap_tc
  bool ret = false;
  // Register initialization function
  jerryx_property_entry methods[] ={ //2 methods
    JERRYX_PROPERTY_STRING_SZ ("id", "mqtt"),
    JERRYX_PROPERTY_FUNCTION ("setServer", js_mqttclient_setserver),
    JERRYX_PROPERTY_FUNCTION ("setCallback", js_mqttclient_setcallback),
    JERRYX_PROPERTY_FUNCTION ("connect", js_mqttclient_connect),
    JERRYX_PROPERTY_FUNCTION ("connected", js_mqttclient_connected),
    JERRYX_PROPERTY_FUNCTION ("loop", js_mqttclient_loop),
    JERRYX_PROPERTY_FUNCTION ("State", js_mqttclient_state),
    JERRYX_PROPERTY_FUNCTION ("publish", js_mqttclient_publish),
    JERRYX_PROPERTY_FUNCTION ("subscribe", js_mqttclient_subscribe),
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
  jerry_value_t prop_name = jerry_string_sz((const char*)"MqttClient"); // 3 Class name
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}
#endif //CMD_MQTT
#endif //_LANG_JERRYSCRIPT_
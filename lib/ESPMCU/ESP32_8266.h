#ifndef __ESP32_8266__H__
#define __ESP32_8266__H__
// Both ESP32 and ESP8266
#include <Arduino.h>
#include "debuglog.h"
#include "ledpwm.h"         //pwm led for ESP32
#include "keyin.h"          // debouce Key in
#include "stdiodep.h"       // stdio dependent on Hardware
#include "littlefsfun.h"    // LittleFS
#include "wifiinit.h"       // Wifi
#include "WebServer.h"      // ESPASYNCWEBServer
#include "audiovideo.h"          // video graphics by winsocket 
#include "config.h"          // video graphics by winsocket 
#ifdef ESP32
#include "timerfun.h"       // for esp32  rtc
#endif 
#endif  //__ESP32_8266__H__
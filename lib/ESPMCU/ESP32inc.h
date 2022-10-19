#ifndef __ESP32INC__H__
#define __ESP32INC__H__
// for ESP32 only 
#include <Arduino.h>
#include "debuglog.h"
#include "ledpwm.h"         //pwm led for ESP32
#include "keyin.h"          // debouce Key in
#include "timerfun.h"       // for esp32  rtc
#include "littlefsfun.h"    // LittleFS
#include "WebServer.h"      // ESPASYNCWEBServer
#endif  //__ESP32INC__H__
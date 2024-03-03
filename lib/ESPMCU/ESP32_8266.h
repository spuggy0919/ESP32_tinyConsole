#ifndef __ESP32_8266__H__
#define __ESP32_8266__H__
// Both ESP32 and ESP8266
#include <Arduino.h>
#include <Freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "debuglog.h"
#include "ledpwm.h"         //pwm led for ESP32
#include "keyin.h"          // debouce Key in
#include "stdiodep.h"       // stdio dependent on Hardware
#include "littlefsfun.h"    // LittleFS
#include "wifiinit.h"       // Wifi
#include "WebServer.h"      // ESPASYNCWEBServer
#include "config.h"          // video graphics by winsocket 
#ifdef ESP32
#include "timerfun.h"       // for esp32  rtc
#include "devices.h"          // mouse touch video audio by winsocket 
#endif 
#endif  //__ESP32_8266__H__
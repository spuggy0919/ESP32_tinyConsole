#ifndef __ESP32INC__H__
#define __ESP32INC__H__
// for ESP32 only 
#include <Arduino.h>
#include <Freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "debuglog.h"
#include "ledpwm.h"         //pwm led for ESP32
#include "stdiodep.h"       // stdio dependent on Hardware
#include "keyin.h"          // debouce Key in
#include "timerfun.h"       // for esp32  rtc
#include "littlefsfun.h"    // LittleFS
#include "wifiinit.h"       // Wifi
#include "WebServer.h"      // ESPASYNCWEBServer
#include "devices.h"          // mouse touch video audio
#include "config.h"          // video graphics by winsocket 
#endif  //__ESP32INC__H__
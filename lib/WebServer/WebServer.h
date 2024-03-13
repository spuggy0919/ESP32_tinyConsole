
#ifndef __WEBSERVER__H__
#define __WEBSERVER__H__


#include <Arduino.h>
#include "wifiinit.h"
#include "littlefsfun.h"
#include "timerfun.h"
#include "ledpwm.h"
#include "wsSerial.h"


#ifdef ESP32
  #include <AsyncTCP.h>
// #define _PROCESSOR_ESP_ "ESP32"
#else
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
#endif
#include <ESPAsyncWebServer.h>
#include "WebSocket.h"
#define SETUP_STATE 0
#define LOOP_STATE  1


void eventSend(String msg,const char * key);
void webEventTask();
void WebServerPage();





// // NTP
// bool WebNTPManage();
// void WebNPTloop();
#endif
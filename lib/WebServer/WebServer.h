
#ifndef __WEBSERVER__H__
#define __WEBSERVER__H__


#include <Arduino.h>
#include "wifiinit.h"
#include "littlefsfun.h"
#include "timerfun.h"
#include "ledpwm.h"
#include "stdiodep.h"


#ifdef ESP32
  #include <AsyncTCP.h>
// #define _PROCESSOR_ESP_ "ESP32"
#else
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
#endif
#include <ESPAsyncWebServer.h>

#define SETUP_STATE 0
#define LOOP_STATE  1

bool WebWSConnect();

void WSSendTXT(String msg);
void WSTransferMessage(int wi,String msg);
void WSTransferChar(int wi,char c);
void WSTransferBufferFlush(int wi);

void eventSend(String msg,const char * key);
void wsTextPrint(String msg);
void wsTextPrintln(String msg);
void wsTextPrint(String msg);
void wsTextPrintCstr(const char *msg);
void wsTextPrintln(String msg);
void WebServerPage();
void webEventTask();
// bool WSTransferBufferTaskInit(int wi);
// bool WSTransferBufferTaskDestroy();

// bool WebNTPPage();
// void WebInputServer();


// // NTP
// bool WebNTPManage();
// void WebNPTloop();
#endif
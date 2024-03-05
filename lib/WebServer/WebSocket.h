
#ifndef __WEBSOCKET__H__
#define __WEBSOCKET__H__
#include "WebServer.h"
#include "Base64.h"
#include "ESP32inc.h"
#include <freertos/task.h>
#include <esp_system.h>
#include "devices.h"


bool WebWSConnect();

void WSSendTXT(String msg);
bool WSSendTXTAck(String msg);
bool WSSendTXTnoAck(String msg);
void WSTransferMessage(int wi,String msg);
void WSTransferChar(int wi,char c);
void WSTransferBufferFlush(int wi);
void wsTextPrint(String msg);
void wsTextPrintln(String msg);
//void wsTextPrintCstr(const char *msg); deprecated use wsTextPrintf
void wsTextPrintf(const char *fmt,...);
void wsMonitorPrintf(const char *fmt,...);

void onWebSocketConnect(AsyncWebSocket *server, AsyncWebSocketClient *client);
void onWebSocketDisConnect(AsyncWebSocket *server, AsyncWebSocketClient *client);

#define DEVICE_AUDIO 8
#define DEVICE_VIDEO 9
void _wsDevice(const char dev,const char *fmt,...);
void WebSocketStart(AsyncWebServer &server);
// bool WSTransferBufferTaskInit(int wi);
// bool WSTransferBufferTaskDestroy();

// bool WebNTPPage();
// void WebInputServer();
#include <list>
extern std::list<String> g_Loopback_Queue;

#endif
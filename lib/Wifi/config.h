#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include "littlefsfun.h"    // LittleFS
#include "wifiinit.h"    // LittleFS

int Config_Init(); // 0:success -1: sta fail -2:AP fail
String Config_Get(String key);
int Config_Remove(String key);
int  Config_Set(String key, String value);
int  Config_SetArgv(String key, int argc, char* argv[]);
String  Config_Data();

#endif
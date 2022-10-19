#ifndef __WIFIINIT_H__
#define __WIFIINIT_H__

#include <Arduino.h>

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
#endif
#define WIFI_AP_MODE 0
#define WIFI_STA_AUTO_MODE 1
#define WIFI_STA_STATIC_MODE 2
#include "WifiSetting.h"

//Variables to save values from HTML form
extern String ssid;
extern String pass;
extern String ip;
extern String gateway;

bool WiFiInit(int mode,...);
bool WiFiSTAAutoIP();
bool WiFiSTAStaticIP(String ssid,String pass,String ip, String gateway);
bool WiFiAP();
#endif
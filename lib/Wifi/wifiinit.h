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
#include "config.h"

//Variables to save values from HTML form
// extern String _ssidname;
// extern String _password;
extern String ip;
extern String gateway;

bool WiFiInit(int mode,...);
bool WiFiSTAAutoIP();
bool WiFiSTAStaticIP(String ssid,String pass,String ip, String gateway);
bool WiFiAP();

// wifi status
String WifiLocalIP();
String WifiSSID();
long WifiRSSI();
int WifiMode();
extern String CurrntClientIP;


#endif
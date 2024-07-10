
/*
 * This file is part of ESP32_TinyConsole.
 *
 * ESP32_TinyConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESP32_TinyConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ESP32_TinyConsole.  If not, see 
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
/*
 * @file config.cpp
 * @brief Wifi Config read config from file system , get two key ssid and password
 *          if ssid and password are not defined, then AP mode  ESOCONSOLE-XXXX
 *          else setup to STA mode and connect to ssid.
 *          Scan can find mDNS name, let user identify.
 * @dependennd   WIFI, LITTLEFS ESPmDNS
 * 
 * @details
 * The functions in this file provide examples for documentation.
 * 
 * @date February 28, 2024
 */


#include "config.h"
const size_t bufferSize = 1024;
JsonDocument jsonDoc;
DeserializationError json_error;
String configData;
/**
 * @brief 
 * @brief Wifi Config read config from file system , get two key ssid and password
 *          if ssid and password are not defined, then AP mode  ESOCONSOLE-XXXX
 *          else setup to STA mode and connect to ssid.
 *          Scan can find mDNS name, let user identify.* 
 * @details
 * This function takes two integers as input and returns their sum.
 * 
 * @param[in] num1 The first integer to be added.
 * @param[in] num2 The second integer to be added.
 * @return 0: for sucessful, -1 -2 (STA Mode fail),,-3 (AP FAIL) fail
 */

int Config_Init()
{
    configData = readFile(LittleFS, "config.json");
    Serial.println(configData);

    // Parse JSON data
    if (!configData.isEmpty()) {
        json_error = deserializeJson(jsonDoc, configData);
    }
    if ((configData.isEmpty()) ||json_error) {
        Serial.println("Failed to parse JSON");
        if (!WiFiInit(WIFI_AP_MODE)){
            Serial.println("Error:Wifi fail");
            return -3; 
        }  
        return 0;
    }
    String runs = Config_Get(String("runs"));
    int runtimes = 1;
    if (!runs.isEmpty()) {
        runtimes=String(runs).toInt();
        runtimes++;
        Config_Set("runs",String(runtimes));
    }

    // Access JSON elements
    String mode = Config_Get(String("wifimode"));
    String ssid = Config_Get(String("ssid"));
    String password = Config_Get(String("password"));
    String ip = Config_Get(String("ip"));
    String gw = Config_Get(String("gw"));
    int wifimode = WIFI_STA_AUTO_MODE;
    mode.toLowerCase();
    if (mode == String("ap")) wifimode = WIFI_AP;
    if (mode ==  String("static")) wifimode = WIFI_STA_STATIC_MODE;

    if (!ssid.isEmpty() ){ // ssid found try Wifi STA //|| !password.isEmpty()
         Serial.printf("mode:%d SSID: %s, Password: %s, ip: %s, gw: %s\n",wifimode, ssid.c_str(), password, ip, gw);
         if (!WiFiInit(wifimode,ssid.c_str(),password.c_str(),ip.c_str(),gw.c_str())){
            Serial.println("Error:Wifi fail, Please reconfig(ssid,password)");
            // Config_Remove("ssid");
            // Config_Remove("password");
            return -2; 
        }  
    }else{
        if (!WiFiInit(WIFI_AP_MODE)) {
            Serial.println("Error:Wifi fail");
            return -1; 
        }  
    }
    return 0;
}
/**
 * @brief Json Get Set Remove and read all data
 * @details
 * This function maintain the json file content.
 * 
 * @param[in] Key
 * @param[in] value  
 * @return if result readback
 */
String Config_Get(String key)
{
    String result = (json_error) ? String(): jsonDoc[key];
    // Serial.println("Get:"+result);
    return result;
}
int Config_Remove(String key)
{   
     String configData;
     jsonDoc.remove(key);
    //  Serial.println("Remove Key:"+key);
    //  Serial.println("Remove configData:"+configData);
     serializeJson(jsonDoc, configData);
    //  Serial.println("Remove configData:"+configData);
     return writeFile(LittleFS,"/config.json",configData.c_str());
}
int  Config_SetArgv(String key, int argc, char* argv[]){

    Config_Remove(key);
     // create an empty array
     JsonArray argvlist = jsonDoc.createNestedArray(key);
     for(int i=0;i<argc;i++){
        argvlist.add(String(argv[i]));
     }
    // serialize the array and send the result to Serial
     serializeJson(jsonDoc, configData);
     return writeFile(LittleFS,"/config.json",configData.c_str());
  
}

int  Config_Set(String key, String value)
{
     String configData;
     jsonDoc[key] = value;
    //  Serial.println("Set configData:"+configData);
     serializeJson(jsonDoc, configData);
    //  Serial.println("Set configData:"+configData);
     return writeFile(LittleFS,"/config.json",configData.c_str());
}
String  Config_Data()
{
    return  readFile(LittleFS, "/config.json");
}
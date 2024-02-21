

#include "config.h"
const size_t bufferSize = 1024;
DynamicJsonDocument jsonDoc(bufferSize);
DeserializationError json_error;
int Config_Init()
{
    String configData = readFile(LittleFS, "config.json");
    // Parse JSON data

    json_error = deserializeJson(jsonDoc, configData);

    if (json_error) {
        Serial.println("Failed to parse JSON");
        if (!WiFiInit(WIFI_AP_MODE)){
            Serial.println("Error:Wifi fail");
            return -3; 
        }  
        return 0;
    }

    // Access JSON elements
    const char* ssid = jsonDoc["ssid"];
    const char* password = jsonDoc["password"];


    if (ssid&&ssid!=""&&password){ // ssid found try Wifi STA
         Serial.printf("SSID: %s, Password: %s\n", ssid, password);
         if (!WiFiInit(WIFI_STA_AUTO_MODE,ssid,password)){
            Serial.println("Error:Wifi fail, Please reconfig(ssid,password)");
            Config_Remove("ssid");
            Config_Remove("password");
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

String Config_Get(String key)
{
    return (json_error) ? String(): jsonDoc[key];
}
int Config_Remove(String key)
{   
     String configData;
     jsonDoc.remove(key);
     serializeJson(jsonDoc, configData);
     return writeFile(LittleFS,"config.json",configData.c_str());
}

int  Config_Set(String key, String value)
{
     String configData;
     jsonDoc[key] = value;
     serializeJson(jsonDoc, configData);
     return writeFile(LittleFS,"config.json",configData.c_str());
}
String  Config_Data()
{
    return  readFile(LittleFS, "config.json");
}
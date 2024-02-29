

#include "config.h"
const size_t bufferSize = 1024;
DynamicJsonDocument jsonDoc(bufferSize);
DeserializationError json_error;
String configData;
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
    String ssid = Config_Get(String("ssid"));
    String password = Config_Get(String("password"));


    if (!ssid.isEmpty() && !password.isEmpty()){ // ssid found try Wifi STA
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
    String result = (json_error) ? String(): jsonDoc[key];
    Serial.println("Get:"+result);
    return result;
}
int Config_Remove(String key)
{   
     String configData;
     jsonDoc.remove(key);
     Serial.println("Remove Key:"+key);
     Serial.println("Remove configData:"+configData);
     serializeJson(jsonDoc, configData);
     Serial.println("Remove configData:"+configData);
     return writeFile(LittleFS,"config.json",configData.c_str());
}

int  Config_Set(String key, String value)
{
     String configData;
     jsonDoc[key] = value;
     Serial.println("Set configData:"+configData);
     serializeJson(jsonDoc, configData);
     Serial.println("Set configData:"+configData);
     return writeFile(LittleFS,"config.json",configData.c_str());
}
String  Config_Data()
{
    return  readFile(LittleFS, "config.json");
}
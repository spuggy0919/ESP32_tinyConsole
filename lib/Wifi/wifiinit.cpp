#include "wifiinit.h"

/* 
  WIFI utility wifi 0.3
  2022/10/19 move setting to WifiSetting.h
  Wifi initialize, mode select from arguments 
  #Function
  WiFiInit(int mode, ...)
  @Arguments
  mode : int 
      #define WIFI_AP_MODE 0
      #define WIFI_STA_AUTO_MODE 1
      #define WIFI_STA_STATIC_MODE 2


  Eaxamples:
  WiFiInit(WIFI_AP_MODE); // AP mode 
  WiFiInit(WIFI_STA_AUTO_MODE, "ssid", "password"); // auto mode 

 */

//Variables to save values from HTML form
String ssidname = WIFISSID;   // modfied for STA mode
String password = PASSWORD; // modfied for STA mode
String ip = LOCAL_IP; // modfied for STA static ip mode
String gateway = GATEWAY; // modfied for STA static ip mode

IPAddress local_IP(192,128,1,200); // modfied for STA static ip mode
// Set your Gateway IP address
IPAddress local_Gateway(192, 168, 1, 1); // modfied for STA static ip mode
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 255, 0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 60000;  // interval to wait for Wi-Fi connection (milliseconds)

#define MACLASTFOUR(mac) (mac.substring(12,14)+mac.substring(15,17))
#define WIFIAPACTUALNAME(mac) (WIFIAPNAME+mac.substring(12,14)+mac.substring(15,17))

bool WiFiInit(int mode,...)
{
  va_list arguments;
  local_IP.fromString(ip);
  local_Gateway.fromString(gateway);
  subnet.fromString(NMASK);
    bool r=false;
    va_start ( arguments, mode );
    switch(mode) {
    case WIFI_AP_MODE:
      r = WiFiAP();
      break;
    case WIFI_STA_AUTO_MODE:
        ssidname = String (va_arg ( arguments, const char* ));
        password = String (va_arg ( arguments, const char* ));
        r = WiFiSTAAutoIP();
      break;
    case WIFI_STA_STATIC_MODE:
        ssidname = String (va_arg ( arguments, const char* ));
        password = String (va_arg ( arguments, const char* ));
        ip = String (va_arg ( arguments, const char* ));
        gateway = String (va_arg ( arguments, const char* ));
        r = WiFiSTAStaticIP(ssidname, password, ip, gateway);
      break;
    default: r=false;
          break;
    }
    va_end ( arguments ); 
    if (!r) {
        r = WiFiAP(); // if fail try AP mode
    }
    if (r) {
      r = WiFimDNS();

    }
    return r;
}
bool WiFiSTAAutoIP() 
{
     Serial.println("WiFiSTA() AutoMode ");
     return WiFiSTAStaticIP(ssidname, password, "", "");
}
bool WiFiSTAStaticIP(String ssid,String pass,String ip, String gateway) {
  Serial.println("WiFiSTAStatic(ssid "+ ssid +",pwd "+ pass +",ip"+ ip +", gw "+ gateway +") ");
  Serial.println("if ip and gateway is null then Auto Sta mode");

  if(ssid=="" ){
    Serial.println("ERROR:Undefined SSID .");
    return false;
  }

  WiFi.mode(WIFI_STA);
  if (ip!="" && gateway!="" ) {
//   for Fixed IP
    Serial.println("Config as Static IP");

    local_IP.fromString(ip.c_str());
    local_Gateway.fromString(gateway.c_str());
  
  
    if (!WiFi.config(local_IP, local_Gateway, subnet)){
      Serial.println("ERROR:STA Failed to configure");
      return false;
    }
  }else{
    Serial.println("Config as auto IP");
  }
  String retries = Config_Get("retries");
  int trycnt = retries.toInt() + 1;
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...try"+String(trycnt));

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    Serial.print(".");
    if (currentMillis - previousMillis >= interval) {

      Config_Set("retries",String(trycnt));
      Serial.println("ERROR:Failed to connect. retry"+String(trycnt)+" config, restart");
      if (trycnt > 3) {
          Serial.println("ERROR:Failed to connect. remove config, restart for AP mode");
          // Config_Remove("ssid");
          // Config_Remove("password");
          return false;
      }
      ESP.restart();
      return false;
    }
    delay(500);
  }
  Config_Remove("retries");
  Serial.println(WiFi.localIP());
  return true;
}

bool WiFiAP() {
  String mac = WiFi.macAddress();

  Serial.println("Initialize Wifi as AP");
  // Serial.println("Warning:This mode will lost some internet hyperlink!");

  // WiFi.disconnect();
  if (!WiFi.softAPConfig(local_Gateway, local_Gateway, subnet)) return false;

  if (!WiFi.softAP((WIFIAPACTUALNAME(mac)).c_str(), PASSWORD)) return false;

  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP:"+WIFIAPACTUALNAME(mac));
  Serial.println("mac:"+mac);
  Serial.print("AP IP address: ");
  Serial.println(IP);
  return true;
}
bool WiFimDNS() 
{
String mac = WiFi.macAddress();
 // Initialize mDNS
  if (!MDNS.begin(WIFIAPACTUALNAME(mac))) {   // Set the hostname to "esp32.local"
    Serial.println("Error setting up MDNS responder!");
    return false;
  }
  return true;
}
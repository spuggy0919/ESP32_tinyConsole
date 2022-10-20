#ifndef __WIFISETTING_H__
#define __WIFISETTING_H__
/* Config your WIFI MODE, SSID, PASSWORD*/

#define WIFISSID    ("SSID")                 // SSID
#define PASSWORD    ("")                  // PASSWORD, default is none.
#define LOCAL_IP    ("192.168.5.2")       // for static sta should be matched with your router NETMASK
#define GATEWAY     ("192.168.5.1")       // for static sta or AP
#define NMASK       ("255.255.255.0")     // for static sta or AP
#define GATEWAY     ("192.168.5.1")       // for static sta or AP
#define WIFIAPNAME  ("ESPCONSOLE-")       // ESPCONSOLE-XXXX for AP will appen four mac chars

/* Select your WIFI MODE, default AP mode*/
#undef WIFISTAAUTO
#undef  WIFISTASTATIC
#define  WIFIAPMODE

#ifdef WIFISTAAUTO
    #define WIFIMODE WIFI_STA_AUTO_MODE
    #undef  LOCAL_IP 
    #define LOCAL_IP ("")
    #undef  GATEWAY  
    #define GATEWAY  ("")
#elif defined(WIFISTASTATIC)
    #define WIFIMODE WIFI_STA_STATIC_MODE
#elif defined(WIFIAPMODE)
    #define WIFIMODE WIFI_AP_MODE
#endif
#endif
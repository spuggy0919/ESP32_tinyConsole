
#include "MPI_lw.h"

bool WiFimDNS_init();
String WifimDNSName();
int WiFimDNSQuery();
#define ESPMPINAME() (String("ESPCONSOLE-")+WiFi.macAddress().substring(12,14)+WiFi.macAddress().substring(15,17))
#define WIFILOCAOIP (WiFi.localIP())

String mdnsName=ESPMPINAME();
const char* _service_name = "esp_tinyconsole";
const char* _service_type = "_http";  // or any other type
const char* _service_protocol = "_tcp";  // or any other type
const uint16_t _service_port = 80;

bool WiFimDNS_init() 
{

String mac = WiFi.macAddress();
 // Initialize mDNS
    MPI_DBG_UDP_PRINTF("[WiFimDNS_init](mac:%s,mdns=%s)\n",mac.c_str(),mdnsName.c_str()); 

  if (!MDNS.begin(mdnsName)) {   // Set the hostname to "esp32.local"
    MPI_DBG_ERROR_PRINTF("Error setting up MDNS responder!");
    return false;
  }
  Serial.println("mdns:"+mdnsName);
   // Advertise service
  MDNS.addService(_service_type, _service_protocol, _service_port);
  return true;
}
int WiFimDNSQuery() 
{
 // Query for services
  int n = MDNS.queryService(_service_type, _service_protocol); // Query for TCP services
  return n;
}
String WifimDNSName(){
  return mdnsName;
}
IPAddress WifimDNSLocalIP(){
    return WiFi.localIP();
}

String MDNS_hostname(int i){
  return MDNS.hostname(i);
}

IPAddress MDNS_IP(int i){
  return MDNS.IP(i);
}
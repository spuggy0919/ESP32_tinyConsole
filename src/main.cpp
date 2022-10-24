/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */

#include "ESP32_8266.h"
#include "interpreter.h"
#include "WifiSetting.h"

#undef LOOP_SSE_TEST

#ifdef LOOP_SSE_TEST 
// SSE variables 
String currentTime;
int triggerCnt = 10;
#endif

int cmd_task(int argc, char *argv[]);

void setup(){
  Serial.begin(115200);
  Serial.println("Web Control");
  pwminit();
  // 1. *** modified your SSID, PASSWORD pair 

  if (!WiFiInit(WIFIMODE, WIFISSID, PASSWORD, LOCAL_IP, GATEWAY)) {
      Serial.println("Error:Wifi fail");
      return ;
  } 
  
  // 2, mount and initalize littlefs for reading page locates in file system

  if(!initLittleFS()){
      Serial.println("Error:LittleFS init fail...");
      return ;
  }  
  // 2.1 IO create Queue
  if (!stdioRedirector()) {
      Serial.println("Error:Websocket Serial not starting...");
  }
  // check LittleFS , it is not necessary 
  // listDir(LittleFS,"/",2);
  // String indexfile = readFile(LittleFS,"/index.html");

  // 3, start up Web Server
  WebServerPage(); // HTTP SSE WS

  // Interpreter
  // WebserForInterpreterSendCmd = interpreterSendCmd; 

  interpreterInit();

 

  // cmd_task(1,(char **)NULL); // testing for task


}


void loop(){


  // if (Serial.available() > 0) { // wait available
  //     backspace is not sent
  //     char c = Serial.read();
  //     Serial.printf("key is %2x %c\n",c,c);
  //  }
  

   interpreter();


 
}
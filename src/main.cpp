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

static int curmillis = millis();

void loop(){


  // if (Serial.available() > 0) { // wait available
  //     backspace is not sent
  //     char c = Serial.read();
  //     Serial.printf("key is %2x %c\n",c,c);
  //  }
  

   interpreter();

#ifdef LOOP_SSE_TEST 

  // similar as log data
  pwmtrigger(500, triggerCnt, (bool)(triggerCnt%2)); // true:pwm, false: no pwm


  // input key counter
  int button = debounceKeyRead(BUTTON_PIN); // read new state  
  if(button == LOW) {
    triggerCnt++; 
    if (triggerCnt>30) triggerCnt = 1;
    // wsTextPrint("E: triggerCnt"+String(triggerCnt));
    eventSend(String(triggerCnt).c_str(),"switchcnt");
    // Serial.printf("The button is pressed(%2d)\r",triggerCnt);
    // toggle state of LED
    pwmled(255);
    eventSend("ON","gpiostate");

  }

  if (KeygetChange()){
     eventSend((button == LOW)? "1" : "0","onoff");
     pwmled((button == LOW)? 255 : 0);
     eventSend((button == LOW)? "ON": "OFF" ,"gpiostate");
  }

  // SSE push event Update
  if (pwmledChange) {
     Serial.println("pwmledChange"+pwmledState);
     eventSend(pwmledState.c_str(),"gpiostate");
    //  wsTextPrint("E:LED "+pwmledState+ " " +String(pwmtriggernum())+ "pwm:" +String(pwmledr()));
  if (!pwmtriggerStopOrNot()){ // trigging
      wsTextPrint(pwmlogData()+" \r");
  }     pwmledChange =false;
  }

#ifdef ESP32
  if ((millis() - curmillis) > 5000  ) {
      curmillis = millis();
      // eventSend(timerCurrent().c_str(),"currenttime");
      String tstr="E:"+timerCurrent()+"\n";
      // WSTransferMessage(1,tstr);
      // if (_d_puttxline((char *)(tstr.c_str()), tstr.length())){
      //      WSTransferBufferFlush(1);
      // } 
  }
#endif 
#endif 
}
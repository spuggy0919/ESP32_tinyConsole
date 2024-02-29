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
bool autoexec_bas();

void setup(){
  Serial.begin(115200);
  Serial.println("Web Control");
  pwminit();
  pwmled(0); // turn off until wifi done
  
  
  // 2, mount and initalize littlefs for reading page locates in file system

  if(!initLittleFS()){
      Serial.println("Error:LittleFS init fail...");
      return ;
  }  

  // check LittleFS , it is not necessary 
  // listDir(LittleFS,"/",2);
  // String indexfile = readFile(LittleFS,"/index.html");

// 1. *** modified your SSID, PASSWORD pair 
//  modified after config file read from littlefs
//  use console export comand to setup
// %export SSID ABCD [enter]
// %export password 12345678 [enter]
// %export [enter] # display the config file content
// if no config file , it will use AP mode , check wifi SSID ESPCONSOLE-XXXX
  int error = Config_Init();
  if (error) {
      Serial.println("Error:Wifi fail:"+String(error));
      return ;
  } 


  // 2, start up Web Server and websocket
  WebServerPage(); // HTTP SSE WS
  // 2.1 IO create Queue
  if (!stdioRedirector()) {
      Serial.println("Error:Websocket Serial not starting...");
      return;
  }
  // Interpreter
  // WebserForInterpreterSendCmd = interpreterSendCmd; 

  // simple shell commands, type "h", "?", "help" in console

  interpreterInit();

  // light up blue led read for connect
  pwmled(255);


  // sometimes reboot ????

  vTaskStartScheduler();
}
int cmd_exec(int argc, char *argv[]);

char  cmd[32]="tb";
#pragma GCC diagnostic ignored "-Wwrite-strings"
char *argvlist[]={
    "exec",cmd
};
/**
 * @brief power on autoexec, *experiment* tb is not object , it can not be reentry.
 * 
 * @details
 * 1. default, if autoexec.bas exist, it run "tb" at background task
 * 2. if autoexec key is defined in config.json, then run this key value.
 *    cmd line is running in Task by InterpreterExcute
 *
 * 
 * @return true, task is running
 */
bool autoexec_bas(){
  String filestr=readFile(LittleFS, "/basic/autoexec.bas");
  String autoexec = Config_Get("autoexec");
  if (autoexec.length()>0){
    strcpy(cmd,autoexec.c_str());
  }
  if (filestr.length()>0) { // autoexec.bas file exist
      cmd_exec(2,argvlist); // for exeample "exec tb", will run autoexec.bas
      return true;
  }
  return false;
}
int autocheck = 0; // 0 disable no autorun exoerment
void loop(){


  // if (Serial.available() > 0) { // wait available
  //     backspace is not sent
  //     char c = Serial.read();
  //     Serial.printf("key is %2x %c\n",c,c);
  //  }
  
  if (autocheck) { // not statble
     // run autoexec.bas 
     autoexec_bas();
     autocheck = 0;
  // cmd_task(1,(char **)NULL); // testing for task
  }
   interpreter();


 
}
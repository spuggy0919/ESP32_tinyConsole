/*
 * This file is part of ESP32_TinyConsole.
 *
 *  Copyright (c) 2022-2024 spuggy0919
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
//            _____     _              _  _                 
//     o O O |_   _|   (_)    _ _     | || |                
//    o        | |     | |   | ' \     \_, |                
//   TS__[O]  _|_|_   _|_|_  |_||_|   _|__/                 
//  {======|_|"""""|_|"""""|_|"""""|_| """"|                
// ./o--000'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'                
//    ___                                      _            
//   / __|    ___    _ _      ___     ___     | |     ___   
//  | (__    / _ \  | ' \    (_-<    / _ \    | |    / -_)  
//   \___|   \___/  |_||_|   /__/_   \___/   _|_|_   \___|  
// _|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""| 
// "`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'

#include "ESP32inc.h"
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
void eof_main();

void setup(){
  Serial.begin(115200);
  Serial.println("TinyConsole "+HTTP_CONSOLE_Version);
  pwminit();
  pwmled(0); // turn off until wifi done
  
  
  // 2, mount and initalize littlefs for reading page locates in file system
#ifdef LITTLEFSFUN
  if(!initLittleFS()){
      Serial.println("Error:LittleFS init fail...");
      return ;
  }  
#else
  if(!g_fsio.init()){
      Serial.println("Error:g_fsio init fail...");
      return ;
  }      
  Serial.println("g_fsio init Done");
#endif
  // check LittleFS , it is not necessary 
  // listDir(LittleFS,"/",2);
  // String indexfile = readFile(LittleFS,"/index.html");
  // 2.1 IO create Queue
  if (!wsSerial.begin()) {
      Serial.println("Error:Websocket Serial not starting...");
      return;
  }
  WSTransferBufferTaskInit(0);
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

  // light up blue led read for connect
  pwmled(255);
  // 2, start up Web Server and websocket
  WebServerPage(); // HTTP SSE WS


  // Interpreter
  // WebserForInterpreterSendCmd = interpreterSendCmd; 

  // simple shell commands, type "h", "?", "help" in console

  interpreterInit();
  char buf[10];
  sprintf(buf,"%8lx",(unsigned int)(&commandTable[0]));
  Serial.println("Command Table loc = "+String(buf));
  sprintf(buf,"%8lx",(unsigned int)(eof_main));
  Serial.println("eof_main = "+String(buf));

  // sometimes reboot ????
//  vTaskStartScheduler(); BUG is will spinlock 
}
int cmd_exec(int argc, char *argv[]);

// cmd should be double quote "" string
#ifdef TINYBASIC
#define AUTORUN "tb /basic/autoexec.bas"
#define AUTORUNFILE "/basic/autoexec.bas"
#else 
#define AUTORUN "js /js/start.js"
#define AUTORUNFILE "/js/start.js"
#endif
#pragma GCC diagnostic ignored "-Wwrite-strings"
char *argvlist[]={
    "exec",AUTORUN
};
/**
 * @brief power on autoexec, *experiment* tb and js are not object , it can not be reentry.
 * 
 * @details
 * 1. default, if autoexec.bas exist, it run "tb" at background task
 * 2. if autorun key is defined in config.json, then run this key value.
 *    cmd line is running in Task by InterpreterExcute
 *
 * 
 * @return true, task is running
 */

int autoexec_bas_js(){
  String filestr=readFile(LittleFS,AUTORUNFILE);
  String autorun = Config_Get("autorun");  // json key autorun 
  // Serial.printf("[main]:autorun(%d,%d) found \n%s\n",2,filestr.length(),filestr);

  if (autorun!="null" && autorun.length()>0){
    String cmd= autorun;
    argvlist[1]=(char *)(cmd.c_str());
    cmd_exec(2,argvlist); // for exeample "exec tb", will run autoexec.bas
    Serial.printf("[main]:autorun(%d,%d) found(%s %s) \n",2,autorun.length(),argvlist[0],argvlist[1]);
    return 2;
  }
  if (filestr.length()>0) { // autoexec.bas file exist
      cmd_exec(2,argvlist); // for exeample "exec tb", will run autoexec.bas
      Serial.printf("[main]:autorun(%d) found(%s %s) \n",1,argvlist[0],argvlist[1]);
      return 1;
  }
  return 0;
}
int autocheck = 1; // 0 disable, no autorun 
void loop(){


  // if (Serial.available() > 0) { // wait available
  //     backspace is not sent
  //     char c = Serial.read();
  //     Serial.printf("key is %2x %c\n",c,c);
  //  }
  
  if (autocheck) { // not statble
     // run autoexec.bas 
     bool ret = autoexec_bas_js();
     autocheck = 0;
  // cmd_task(1,(char **)NULL); // testing for task
  }
   interpreter();


 
}
void eof_main(){

}
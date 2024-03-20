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
#include "interpreter.h"

#define INTERPRETER_STARTUP 0
#define INTERPRETER_READY 1
#define INTERPRETER_WAITING 2
#define INTERPRETER_RUNNING 3
#define INTERPRETER_DONE 4




#define CMDLINE_BUFFERSIZE 128 


bool InterpreterCmdIn = false;
int InterpreterCmdIndex;
unsigned int InterpreterCmdOption = 0;


int interpreterState =  INTERPRETER_STARTUP;
char cmdline[CMDLINE_BUFFERSIZE]; // for CommandParser will be deleted
#define ARGVPTR_SIZE 20

String interpreterCmdBuf = "";
int argc = 0;
char *argv[ARGVPTR_SIZE];
static char argvline[CMDLINE_BUFFERSIZE];
int separate (
    String str,
    char   **p,
    int    size )
{
    int  i,n,dcnt=0,len;
    char delimiter = 0,c,*p1;
    strcpy (argvline, str.c_str ());
    wsMonitorPrintf("argv=%s\n",argvline);
    p1=argvline;
    for(i=0;*p1!=0;i++,p1++){
        c=*p1;
        if (delimiter==0&&(c=='"'))  { dcnt++; delimiter = c; *p1=' '; continue; }// find first string delimiter
        if (delimiter!=0&&(c==delimiter))  { dcnt++; delimiter = 0; *p1=' '; continue;}// find first string delimiter
        if (c=='\\')  {  i++; c=*p1++; }// skip escape sequence
        if ((dcnt%2)&&c==' ') *p1=0xff; // change to esc;
    }
    len = i;
    *p++ = strtok (argvline, " \n");
    for (n = 1; NULL != (*p++ = strtok (NULL, " \n")); n++){
        if (size == n)
            break;
    }
    p1=argvline;
    for(i=0;i<len;i++){
        if (p1[i]==0xff) p1[i]=' ';
    }
       // if (size>0 && p[size-1][strlen(p[size-1])-1]=='\n') p[size-1][strlen(p[size-1])-1]= 0;
    return n; // trimming space \n  in getline

}
void parsercmdline(String line){
     argc = 0;
     if (line == "" || line == "\n") return;
     argc = separate (line, argv, ARGVPTR_SIZE);
     Serial.printf("Parser command Line argc=%d\n",argc);
    //  for(int i=0;i<argc;i++)
    //    Serial.printf("argv[%d]=%s\n",i,argv[i]);
    //   for(int i=0;i<strlen(argv[0]);i++)
    //        Serial.printf("argv0[%2x]%c\n",argv[i]);

}
int InterpreterExcute(String *cmd){
  int i;
  parsercmdline(*cmd);
  Serial.printf("Command:argc=%d, %s found!", argc, argv[0]);
  String cmdStr = String(argv[0]);
  cmdStr.toLowerCase();
  for(i=1;i<CMDNUMBERS;i++){
      if (String(commandTable[i].funName) == "-1" || argc == 0) break;
      if (String(commandTable[i].funName).startsWith(cmdStr)) {
         InterpreterCmdIndex = i;
         wsTextPrint("\n"); // newline for console
         Serial.printf("Command:%s match!",commandTable[i].funName);
         cmd_option(argc,argv); // check option exist or not
         int ret=(*(commandTable[i].cmdPtr))(argc,argv);
          *cmd = "";


        //  Serial.printf(" %d\n",ret);
         return ret;
      // }else{
      //     cmd_prompt(argc,argv);
      }
  } 
  *cmd = "";
  if (argc>0) wsTextPrint("\nillegal cmd, try help\n%");
  return -1;
}
bool interpreterCheckReceiveCmd(String cmd){
    // it is impossible to avoid race around maybe! 
    if   (interpreterCmdBuf == "" ){
        interpreterCmdBuf = cmd.substring(0,cmd.length()); // delete enter?
        Serial.printf("Interpreter Receive command:%s[%d]\n",interpreterCmdBuf.c_str(),interpreterCmdBuf.length());
        Serial.printf("Interpreter command:%s[%d]\n",interpreterCmdBuf.c_str(),interpreterCmdBuf.length());

        return true;
 

    }
    return false;
}

const char *statemsg[]={
"INTERPRETER_STARTUP",
"INTERPRETER_READY",
"INTERPRETER_WAITING",
"INTERPRETER_RUNNING",
"INTERPRETER_DONE",
};
#define LINE_LIMIT 256
char buf[LINE_LIMIT];
int len = 0;
String BannerTinyConsoleCopyRightNotice =" \
ESP32 Websocket TinyConsole("+HTTP_CONSOLE_Version+") 2022/10/19 \n \
https://github.com/spuggy0919/ESP32_tinyConsole.git \n \
(GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007) \n \
Author:spuggy0919 \n  \
";

String BannerMessageWithTinyBasicCopyRightNotice ="\n \
/* \n \
 *	$Id: basic.c,v 1.138 2022/08/15 18:08:56 stefan Exp stefan $ \n \
 * \n \
 *	Stefan's IoT BASIC interpreter \n \
 * \n \
 * 	See the licence file on \n \
 *	https://github.com/slviajero/tinybasic for copyright/left.\n \
 *    (GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007)\n \
 *\n \
 *	Author: Stefan Lenz, sl001@serverfabrik.de\n \
 */ \n\
 Press any key to continue, try help, ? or h ";
int interpreter() {
    int ret;
    // Serial.printf("State:%s\n",statemsg[interpreterState]);
    switch(interpreterState) {
      case INTERPRETER_STARTUP:
           if (!WebWSConnect()) break;
#ifdef TINYBASIC
           wsTextPrintln(BannerTinyConsoleCopyRightNotice);
           wsTextPrintln("Buildin TinyBasic with below");
           wsTextPrintln(BannerMessageWithTinyBasicCopyRightNotice);
            wsSerial.getChar();
#endif
           cmd_cls(argc,argv); 
           wsTextPrintln("ESP32 Websocket TinyConsole" +  HTTP_CONSOLE_Version);
           wsTextPrintln("Author:spuggy0919");
          wsSerial.write('\n');
          wsSerial.write('%');
          interpreterCmdBuf = "";
          interpreterState = INTERPRETER_WAITING;
          break;
       case INTERPRETER_READY:
          wsSerial.write('\n');
          wsSerial.write('%');
          interpreterCmdBuf = "";
          interpreterState = INTERPRETER_WAITING;
          break;
       case INTERPRETER_WAITING:
           if (wsSerial.readLine(buf, &len)){
              // printhexLong(buf, len);
              if (buf[0]=='\n') {
                Serial.printf("%d\n",len);
                interpreterState = INTERPRETER_READY;
                return 0;
              }
              if (interpreterCheckReceiveCmd(String(buf))){
                  interpreterState = INTERPRETER_RUNNING;
                  return 0;
              }; 

          }         
          break;
       case INTERPRETER_RUNNING:  
          Serial.printf("Interpreter Running...%s\n",interpreterCmdBuf.c_str());
          ret = InterpreterExcute(&interpreterCmdBuf);
          if (ret!=0) Serial.println("fail");
          interpreterState = INTERPRETER_DONE;
          break;
       case INTERPRETER_DONE:
          interpreterState = INTERPRETER_READY;
          break;
       default:
           interpreterCmdBuf = "";
          interpreterState = INTERPRETER_READY;
          break;   
    }
    return 0;
}
int cmd_chkfileorPath(int argc, char *argv){
  int file1 = 1;
  int file2 = 2;
  if (InterpreterCmdOption!=0) {
    file1++; file2++;
  }
  return 0;
}
unsigned long curmillis = millis();
int cmd_millis(int argc, char *argv[]){
    char buffer[20];
    sprintf(buffer, "%ul\n", millis());
    wsTextPrintln(String(buffer));
    return 0;
}   

int cmd_millisdelay(int argc, char *argv[]){
    if (argc >=2 && argv[1]!=NULL) {
        int delay = (unsigned int ) strtoul(argv[1],0,10);
        vTaskDelay( delay / portTICK_PERIOD_MS ); 
    }
    return true;
}
int cmd_option(int argc, char *argv[]){
  // check optionexist or not 
  InterpreterCmdOption = 0;
   if (argv==NULL) return false;
   if (argc < 2 ) return false;
   char c=argv[1][0];
   char *p=argv[1];
   if (c=='-') {  // option found
      InterpreterCmdOption = COMMAND_OPTION_EXIST;
      c = *p; p++;
      while(c){
          switch(*p){
          case 'h':
          case '?':
              wsTextPrintln("Oh Oh SOS ....\n");
              wsTextPrintln(String(commandTable[InterpreterCmdIndex].description));
              break;
          case 'v':
              wsTextPrint("ESP CONSOLE: version "+ HTTP_CONSOLE_Version);
              break;
          case 'r':
          case 'R':
            InterpreterCmdOption |= COMMAND_RECURIVE_DIRECTORY;
            break;
          }
          c = *p++;
     }
   }
   return (InterpreterCmdOption!=0);
}

int cmd_test(int argc,char * argv[]) {
  wsTextPrint("WebSocket: "); if (WebWSConnect()) wsTextPrintln(String(("true")));
  // wsTextPrint("string: "); wsTextPrintln(String((argv[0])));
  // wsTextPrint("double: "); wsTextPrintln(String(argv[2].asDouble));
  // wsTextPrint("int64: "); wsTextPrintln(String(((int32_t)args[2].asInt64))); // NOTE: on older AVR-based boards, Serial doesn't support printing 64-bit values, so we'll cast it down to 32-bit
  // wsTextPrint("uint64: "); wsTextPrintln(String((uint32_t)args[3].asUInt64)); // NOTE: on older AVR-based boards, Serial doesn't support printing 64-bit values, so we'll cast it down to 32-bit
  test_main(argc,argv);   // * testmain
  return 0;

}

extern void setup();
extern void loop();
extern void pwmled(int dutycycle);
int cmd_pio(int argc,char * argv[]){
  if (argc<=1) return -1;
    Serial.printf("setup address %lx\n", setup);
    Serial.printf("loop address %lx\n", loop);
    Serial.printf("pwmled address %lx\n", pwmled);
    void (*f)(int) = pwmled;
    (*f)(atoi(argv[1]));
    for(int i=1; i<3;i++) {
      Serial.printf("table address %lx, %lx %s\n", commandTable[i].cmdPtr,commandTable[i].cmdPtr,commandTable[i].funName );
    }
    return 0;
}
int cmd_help(int argc,char * argv[]){
    for (int i=1;i<CMDNUMBERS;i++){
       wsTextPrintln(String(commandTable[i].funName) + " " + String(commandTable[i].description));
       if (commandTable[i].funName[0] == '?') break;
    }
#ifdef TINYBASIC
    wsTextPrintln("\nIn TinyBasic, Running break press '#'");
    wsTextPrintln("Exit TinyBasic,  press 'CTRL-C' button");
#endif
    wsTextPrintln("SyncTime Button set esp32 rtc with client time");
    wsTextPrintln("Clear Button clear monitor, same as cls");
    wsTextPrintln("Upload Button upload file to EPS32");
    wsTextPrintln("Graphics checkbox display canvas or none");
    return 0;
}
int cmd_echo(int argc,char * argv[]){
    if (argc<=1) return -1;
  
    wsTextPrintln(String(argv[1]));
    return 0;
}
int cmd_download(int argc,char * argv[]){
  if (argc<=1) return -1;
  String paths =  getfullpathFileOrDir(LittleFS, argv[1]);
  eventDownloadFile(paths); 
    return 0;

}
int cmd_reboot(int argc,char * argv[]){

  pwmled(0);

  ESP.restart(); return 0;
}
int cmd_status(int argc,char * argv[]){
     return interpreterState;
}
// int cmd_exist(int argc,char * argv[]){
//     String ret=getFullPath_File("/basic/", "autoexec.bas"){
//     if (ret!=""){
//     }
//     return 0;
// }
// const functionTable = [ detail see javascripts
//     createCard,   "0"
//     drawFilledRect,    // 1 drawFilledRect(color, x, y, width, height)
//     drawPixel,         // 2  drawPixel(color, x, y)
//     drawText,          // 3 drawText(color, text, x, y, deg) 
//     drawHLine,         // 4 drawHLine(color, x1, x2, y)
//     drawVLine,         // 5 drawVLine(color, x, y1, y2
//     plotto,            // 6 drawLine(color, x1, y1, x2, y2)
// ];
int cmd_video(int argc,char * argv[]){
    video(argc,argv);
    return 0;
}
static int videoflag=0;
int cmd_avtest(int argc,char * argv[]){


    if (argc==1) {
        audiof("0"); // play test.mp3
        for(int x=0; x<640/2; x+=20){
          for(int y=x; y<480/2; y+=20){
            unsigned int r = random(255);
            unsigned int g = random(255);
            unsigned int b = random(255);
            unsigned int a = random(255);
            unsigned int color = (a)|  (r<<24)| (g<<16)|(b<<8);
            drawPenColor(color);  
            drawFRect(x,y,(640-x*2),(480-y*2));
            // delay(10);
          }
        }
        for(int y=0; y<480/2; y+=20){
          for(int x=y; x<640/2; x+=20){
            unsigned int a = random(255);
            unsigned int b = random(255);
            unsigned int g = random(255);
            unsigned int r = random(255);
            unsigned int color = (a<<24)|  (r<<16)| (g<<8)|(b);
            drawPenColor(color);  
            drawFRect(x,y,(640-x*2),(480-y*2));
            // delay(10);
          }
        }
    }else{
      char c=argv[1][0];
      // consoloLog("avtest %d\n",c-'0');

      switch(c) {
        case '0': {// clear
          int color = 0xffff00FF;
          drawPenColor(color);  

          String cmd;
          cmd = String("1 ");
          cmd += String(0)+ " ";
          cmd += String(0)+ " ";
          cmd += String(640)+ " ";
          cmd += String(480);          
          videof(cmd.c_str());
          // delay(100);
          }break;
        case '1': {// FCIRCLE
         for (int i=0;i<500;i++){
            unsigned int a = random(255);
            unsigned int b = random(255);
            unsigned int g = random(255);
            unsigned int r = random(255);
            unsigned int color = (a)|  (r<<24)| (g<<16)|(b<<8);
            drawPenColor(color);  

            int x=random(639);
            int y=random(480);
            r=random(50);
            drawFCircle(x,y,r);
            // delay(100);       
          }
          }break;
        case '2': // pixel
        default:
          for (int i=0;i<500;i++){
            int b = random(255);
            int g = random(255);
            int r = random(255);
            unsigned int color = (0xff)|  (r<<24)| (g<<16)|(b<<8);
            drawStrokeWidth(5);  
            drawPenColor(color);  

            int x=random(639);
            int y=random(480);
            String cmd;
            cmd = String("2 ");
            cmd += String(x)+ " ";
            cmd += String(y);
            videof(cmd.c_str());
            // delay(100);       
          }
          break;       
        case '4': // HLine / Vline
          for(int y=0; y<480/2; y+=20){
              int b = random(255);
              int g = random(255);
              int r = random(255);
            unsigned int color =  (r<<24)| (g<<16)|(b<<8)|0xff;
            drawStrokeWidth(5);  
            drawPenColor(color);  
              String cmd;
              cmd = String("4 ");
              cmd += String(y)+ " ";
              cmd += String(640-y*2)+ " ";
              cmd += String(y)+ " ";
              videof(cmd.c_str());
              cmd = String("5 ");
              cmd += String(y)+ " ";
              cmd += String(480-y*2)+ " ";
              cmd += String(y);
              videof(cmd.c_str());           
              // delay(100);
          }
          break;
 
        case '6': // TEXT
        Serial.println("drawText");
          for(int y=0; y<480; y+=19){
              int b = random(255);
              int g = random(255);
              int r = random(255);
            unsigned int color =  (r<<24)| (g<<16)|(b<<8)|0xff;
              drawSetFontColor(color);  
              drawSetFontSize(50);  
              drawText("ABC DEF",0,y,0); 
              // delay(50);
            }
          break;
        case '5': // Char
        Serial.println("drawChar");
          for(int y=0; y<300; y+=1){
              int b = random(255);
              int g = random(255);
              int r = random(255);
            unsigned int color =  (r<<24)| (g<<16)|(b<<8)|0xff;
              drawSetFontColor(color);  
              drawSetFontSize(20); 
               char c=((y/80)%25)+'A';
              drawChar(c); 
              // delay(10);
            }
          break;
      }        
    }
    

    return interpreterState;

}

// export 
int cmd_export(int argc,char * argv[]){
    if (argc==1){
        wsTextPrint(Config_Data());
    }if (argc==2) {
          return Config_Remove(String(argv[1]));
    }else{
       if (argc==3)
          return Config_Set(String(argv[1]),String(argv[2]));
    }
    return 0;
}

int cmd_ping(int argc,char * argv[]){
    String str= String(argv[1]);
    // if (str.toUpperCase() == String("ON")){
        
    // }
    return 0;
}



void interpreterInit() {
  

}
int cmd_cls(int argc,char * argv[]){

    wsTextPrintf("\x1b[2J");
    if (argc>1){
          int fg =  strtoul(argv[1], NULL, 16);
          // drawPenColor(0xff);  // black;
          // drawFRect(0,0,(640),(480));
          drawPenColor(fg);  // ;
          drawFRect(0,0,(640),(480));    }

    return 0;
}
int cmd_prompt(int argc,char * argv[]){
    Serial.printf("cmd_prompt %8lx\n",(unsigned long)cmd_prompt);
    Serial.printf("COMMAND_TABLE %8lx\n",(unsigned long)commandTable);
    wsTextPrint("\n%");
    return 0;
}


int cmd_call(int argc,char * argv[]){
    int ret=-1;
    argc--;
    unsigned long idx =  strtoul(argv[1], 0, 16);
    if (idx < CMDNUMBERS && idx!=0 ) {
       ret = (*(commandTable[(int)idx].cmdPtr)) (argc, &argv[1]);
    }else{
      MAINPTR cmdptr = (MAINPTR) idx;
      Serial.printf("cmdptr=%8lx",idx);
      ret = (*cmdptr)(argc, &argv[2]);
    }
     return ret;
}


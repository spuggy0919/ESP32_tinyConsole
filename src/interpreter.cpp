/*
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
int cmd_help(int argc,char * argv[]);
int cmd_echo(int argc,char * argv[]);
int cmd_pwd(int argc,char * argv[]);
int cmd_time(int argc,char * argv[]);
int cmd_cd(int argc,char * argv[]);
int cmd_fp(int argc,char * argv[]);
int cmd_append(int argc,char * argv[]);
int cmd_write(int argc,char * argv[]);
int cmd_ls(int argc,char * argv[]);
int cmd_ls0(int argc,char * argv[]);
int cmd_df(int argc,char * argv[]);
int cmd_mkdir(int argc,char * argv[]);
int cmd_rmdir(int argc,char * argv[]);
int cmd_cat(int argc,char * argv[]);
int cmd_rm(int argc,char * argv[]);
int cmd_download(int argc,char * argv[]);
int cmd_cp(int argc,char * argv[]);
int cmd_mv(int argc,char * argv[]);
int cmd_status(int argc,char * argv[]);
int cmd_tinybasic(int argc,char * argv[]);
int cmd_ping(int argc,char * argv[]);
int cmd_prompt(int argc,char * argv[]);
int cmd_call(int argc,char * argv[]);
int cmd_dumpHex(int argc,char * argv[]);
int cmd_cls(int argc,char * argv[]);
int cmd_taskdelay(int argc, char *argv[]);
int cmd_millisdelay(int argc, char *argv[]);
int cmd_millis(int argc, char *argv[]);
int cmd_option(int argc, char *argv[]);
int cmd_video(int argc, char *argv[]);
int cmd_avtest(int argc, char *argv[]);
int cmd_export(int argc, char *argv[]);
int cmd_reboot(int argc,char * argv[]);

typedef int (*MAINPTR)(int argc,char * argv[]);

typedef struct COMMAND_TABLE_ITEM{
   String   funName;
   MAINPTR  cmdPtr;
   String   description;
} COMMAND_TABLE;

COMMAND_TABLE commandTable[]= {
  "tb",     cmd_tinybasic,  "file.bas\t// tinyBasic run file",
  "tinybasic",  cmd_tinybasic,  "\t// tinyBasic interactive",
  "pwd",    cmd_pwd,    "\t\t// current path",
  "cd",     cmd_cd,     "\t\t// change directory",
  "fp",     cmd_fp,     "path \t// full path testing",
  // "ls0",    cmd_ls0,    "      // dummy list",
  "df",     cmd_df,     "\t\t// disk infomation",
  "ls",     cmd_ls,     "path  \t// list directory",
  "mkdir",  cmd_mkdir,  "path  \t// make directory ",
  "rmdir",  cmd_rmdir,  "path  \t// remove directory",
  "cat",    cmd_cat,    "file  \t// diplay file content",
  "rm",     cmd_rm,     "file  \t// remove file",
  "echo",   cmd_echo,   "msg   \t// echo message",
  "write",  cmd_write,  "file msg \t// write message" ,
  "append", cmd_append, "file msg // append message",
  "cp",     cmd_cp,     "file1 file2 \t// copy file1 to file2",
  "mv",     cmd_mv,     "file1 file2 \t// rename file1 to file2",
  "time",   cmd_time,   "\t\t// display currnet time",
  "pio",    cmd_pio,    "0~255 \t// set LED pin output pwm value",
  "dl",     cmd_download,"file  \t// download file to PC",
  "reboot", cmd_reboot,  "\t\t// reboot",
  "ping",   cmd_test,   "\t\t// ping test, int in monitor",
  "cls",    cmd_cls,    "\t\t// clear screen",
 "avtest",  cmd_avtest, "[0|1|2]\t// video graphic testing",
 "export",  cmd_export, "\t\t// export ssid ABC \n \t\t// export password 12345678 \n\t\t// export\n\t\t// export all wifi config or set config", 
 "loop",    cmd_test,    "\t\t// loopback test and arglist parser Test",
  "exec",   cmd_exec,   "\t\t// exec \"cmd argv[]\" for task experiment",
  "kill",   cmd_kill,   "\t\t// kill process, for task experiment ",
  "ps",     cmd_ps,     "\t\t// process status, for task experiment",
"help",     cmd_help,   "\t\t// help ",
  "h",      cmd_help,   "\t\t// help",
  "?",      cmd_help,   "\t\t// help",
 //  below internal teing commands
  "status", cmd_status,  "status",
"test",   cmd_test,    "\t// dummy command",
"task",    cmd_task,  "  // tasktest testing...",
 "dump",    cmd_dumpHex,   "0xaddr len //  dump Physical memory testing...",
 "call",    cmd_call,     "address argv ...// call cmd function  testing...",
 "tdelay",  cmd_taskdelay,     "delaynum...// for wssockettask idle time...",
 "option",  cmd_option,     "arglist // for '-' option check...",
 "millis",  cmd_millis,     " // for '-' option check...",
 "mdelay",  cmd_millisdelay,     "delaycnt // for '-' option check...",
 "video",  cmd_video,       "video 0  // video graphics functions",
 "hello",  cmd_hello,       "jmp hello",

  "-",      cmd_prompt,   "",
   "",      cmd_prompt,   "",
};
#define CMDNUMBERS (sizeof(commandTable)/sizeof(COMMAND_TABLE))

#define CMDLINE_BUFFERSIZE 128 
int test_main(int argc,char * argv[]);


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
  for(i=0;i<CMDNUMBERS;i++){
      if (commandTable[i].funName == "-1" || argc == 0) break;
      // if (String(argv[0]).equalsIgnoreCase(commandTable[i].funName)) {
      if (commandTable[i].funName.startsWith(cmdStr)) {
         InterpreterCmdIndex = i;
         wsTextPrint("\n"); // newline for console
         Serial.printf("Command:%s found!",argv[0]);
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
char buf[LINE_LIMIT];
int len = 0;
String BannerTinyConsoleCopyRightNotice =" \
ESP32 Websocket TinyConsole(%s) 2022/10/19 \n \
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
           sprintf(buf, BannerTinyConsoleCopyRightNotice.c_str() , HTTP_CONSOLE_Version);
           wsTextPrintln(String(buf));
           wsTextPrintln("Buildin TinyBasic with below");
           wsTextPrintln(BannerMessageWithTinyBasicCopyRightNotice);
           _d_GetChar();
           cmd_cls(argc,argv); 
           wsTextPrintln("ESP32 Websocket TinyConsole" +  HTTP_CONSOLE_Version);
           wsTextPrintln("Author:spuggy0919");
          wsTextPrint("\n%");
          interpreterCmdBuf = "";
          interpreterState = INTERPRETER_WAITING;
          break;
       case INTERPRETER_READY:
          wsTextPrint("\n%");
          interpreterCmdBuf = "";
          interpreterState = INTERPRETER_WAITING;
          break;
       case INTERPRETER_WAITING:
          if (_d_getrxline(buf, &len)){
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
    Serial.printf("setup address %lx\n", setup);
    Serial.printf("loop address %lx\n", loop);
    Serial.printf("pwmled address %lx\n", pwmled);
    void (*f)(int) = pwmled;
    (*f)(atoi(argv[1]));
    for(int i=0; i<3;i++) {
      Serial.printf("table address %lx, %lx %s\n", commandTable[i].cmdPtr,commandTable[i].cmdPtr,commandTable[i].funName );
    }
    return 0;
}
int cmd_help(int argc,char * argv[]){
    for (int i=0;i<CMDNUMBERS;i++){
       wsTextPrintln(commandTable[i].funName + " " + commandTable[i].description);
       if (commandTable[i].funName[0] == '?') break;
    }
    wsTextPrintln("\nIn TinyBasic, Running break press '#'");
    wsTextPrintln("Exit TinyBasic,  press 'CTRL-C' button");
    wsTextPrintln("SyncTime Button set esp32 rtc with client time");
    wsTextPrintln("Clear Button clear monitor, same as cls");
    wsTextPrintln("Upload Button upload file to EPS32");
    wsTextPrintln("Graphics checkbox display canvas or none");
    return 0;
}
int cmd_time(int argc,char * argv[]){
    wsTextPrintln(timerCurrent()+"\n%");
      return 0;
}
int cmd_echo(int argc,char * argv[]){
    
    wsTextPrintln(String(argv[1]));
    return 0;
}
int cmd_pwd(int argc,char * argv[]){
    wsTextPrintln(currentDir());
    return 0;
}

int cmd_cd(int argc,char * argv[]){
  wsTextPrintln(changeDir(LittleFS,argv[1]));
  return 0;

}
int cmd_fp(int argc,char * argv[]){
  wsTextPrintln(getfullpathFileOrDir(LittleFS, argv[1]));
  return 0;
  
}
int cmd_append(int argc,char * argv[]){
  appendFile(LittleFS, argv[1], argv[2]);
  String data = readFile(LittleFS, argv[1]);
  wsTextPrintln(data.c_str());
   return 0;
 
}
int cmd_write(int argc,char * argv[]){
      writeFile(LittleFS, argv[1], argv[2]);
      String data = readFile(LittleFS, argv[1]);

   wsTextPrintln(data.c_str());
   return 0;
 
}
int cmd_ls(int argc,char * argv[]){
  String path=String(argv[1]);
  int level=1;
  if (argc>=3 && atoi(argv[2])>level) level =atoi(argv[2]);
  wsTextPrintln(listDir(LittleFS, argv[1], level));
  return 0;

}

int cmd_ls0(int argc,char * argv[]){
  String path=currentDir();
  wsTextPrintln(listDir(LittleFS, path.c_str(), 1));
    return 0;

}
#include <esp_partition.h>
int cmd_df(int argc,char * argv[]){
  char sbuf[256];
  sprintf(sbuf,"ESP32 ChipIModel=%s\nRevision=%x\nChipCores=%d\nClockFreq==%uMZ ",ESP.getChipModel(),ESP.getChipCores(),ESP.getCpuFreqMHz());
  wsTextPrintln(sbuf);
  sprintf(sbuf,"FlashSize=0x%x\nFlashChipSpeed=%u\nFlashChipMode=%x ",ESP.getFlashChipSize(),ESP.getFlashChipSpeed(),ESP.getFlashChipMode());
  wsTextPrintln(sbuf);
  sprintf(sbuf,"PsramSize=%u\nFreePsram=%u\nMinFreePsram=%u\nMaxAllocPsram=%u ",ESP.getPsramSize(),ESP.getFreePsram(),ESP.getMinFreePsram(),ESP.getMaxAllocPsram());
  wsTextPrintln(sbuf);
  wsTextPrintln(reportfs(LittleFS).c_str());
 // Retrieve the partition table
  esp_partition_iterator_t iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  while (iterator) {
    const esp_partition_t *partition = esp_partition_get(iterator);
    
    sprintf(sbuf,"Partition label: %s, Type: %d, Subtype: %d, Size: %x bytes\n",
                  partition->label, partition->type, partition->subtype, partition->size);
    wsTextPrint(sbuf);
    iterator = esp_partition_next(iterator);
  }
    return 0;

}
int cmd_mkdir(int argc,char * argv[]){
  createDir(LittleFS, argv[1]);
    return 0;

}
int cmd_rmdir(int argc,char * argv[]){
  removeDir(LittleFS, argv[1]);
    return 0;

}
int cmd_cat(int argc,char * argv[]){
  wsTextPrint(readFile(LittleFS, argv[1]));
  wsTextPrintln("\n");
    return 0;

}
int cmd_rm(int argc,char * argv[]){
  deleteFile(LittleFS, argv[1]);
    return 0;

}
int cmd_download(int argc,char * argv[]){
  String paths =  getfullpathFileOrDir(LittleFS, argv[1]);
  eventDownloadFile(paths); 
    return 0;

}
int cmd_cp(int argc,char * argv[]){
  String data = readFile(LittleFS, argv[1]);
  writeFile(LittleFS, argv[2], data.c_str());
  return 0;
}
int cmd_mv(int argc,char * argv[]){
  int ret = cmd_cp(argc,argv);
  if (!ret) return ret;
  ret = cmd_rm(argc,argv);
  return ret;
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
            unsigned int a = random(255);
            unsigned int b = random(255);
            unsigned int g = random(255);
            unsigned int r = random(255);
            unsigned int color = (a<<24)|  (r<<16)| (g<<8)|(b);
            drawPenColor(color);  
            drawFRect(x,y,(640-x*2),(480-y*2));
            delay(10);
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
            delay(10);
          }
        }
    }else{
      char c=argv[1][0];
      // consoloLog("avtest %d\n",c-'0');

      switch(c) {
        case '0': {// clear
          int color = 0xffff00;
          drawPenColor(color);  

          String cmd;
          cmd = String("1 ");
          cmd += String(0)+ " ";
          cmd += String(0)+ " ";
          cmd += String(640)+ " ";
          cmd += String(480);          
          videof(cmd.c_str());
          delay(100);
          }break;
        case '1': {// FCIRCLE
         for (int i=0;i<500;i++){
            unsigned int a = random(255);
            unsigned int b = random(255);
            unsigned int g = random(255);
            unsigned int r = random(255);
            unsigned int color = (a<<24)|  (r<<16)| (g<<8)|(b);
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
            int color = (r<<16)| (g<<8)|(b);
            drawPenColor(color);  

            int x=random(639);
            int y=random(480);
            String cmd;
            cmd = String("2 ");
            cmd += String(x)+ " ";
            cmd += String(y);
            videof(cmd.c_str());
            delay(100);       
          }
          break;       
        case '4': // HLine / Vline
          for(int y=0; y<480/2; y+=20){
              int b = random(255);
              int g = random(255);
              int r = random(255);
              int color = (r<<16)| (g<<8)|(b);
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
              delay(100);
          }
          break;
 
        case '6': // TEXT
        Serial.println("drawText");
          for(int y=0; y<480; y+=19){
              int b = random(255);
              int g = random(255);
              int r = random(255);
              int color = (r<<16)| (g<<8)|(b);
              drawPenColor(color);  
              drawText("ABC DEF",0,y,0); 
              delay(50);
            }
          break;
        case '5': // Char
        Serial.println("drawChar");
          for(int y=0; y<3000; y+=1){
              int b = random(255);
              int g = random(255);
              int r = random(255);
              int color = (r<<16)| (g<<8)|(b);
              drawPenColor(color);  
              char c=((y/80)%25)+'A';
              drawChar(c); 
              delay(10);
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


int cmd_tinybasic(int argc,char * argv[]){
  int i=0;
 //WSTransferBuffer(&i); // taskIO
      // WSTransferBufferTaskInit(0);
      // tbsetup();
      int ret = tbmain(argc,argv);
      // WSTransferBufferTaskDestroy();
      return ret;
}
void interpreterInit() {
  

}
int cmd_cls(int argc,char * argv[]){

    wsTextPrintf("\x1b[2J");
    // drawPenColor(0);  
    // drawFRect(0,0,(640),(480));
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
    if (idx < CMDNUMBERS ) {
       ret = (*(commandTable[(int)idx].cmdPtr)) (argc, &argv[1]);
    }else{
      MAINPTR cmdptr = (MAINPTR) idx;
      Serial.printf("cmdptr=%8lx",idx);
      ret = (*cmdptr)(argc, &argv[2]);
    }
     return ret;
}


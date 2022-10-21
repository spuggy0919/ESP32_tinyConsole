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

typedef int (*MAINPTR)(int argc,char * argv[]);

typedef struct COMMAND_TABLE_ITEM{
   String   funName;
   MAINPTR  cmdPtr;
   String   description;
} COMMAND_TABLE;

COMMAND_TABLE commandTable[]= {
  // "TEST",   cmd_test,   "      // dummy command",
  "pwd",    cmd_pwd,    "      // current path",
  "cd",     cmd_cd,     " dir  //change directory",
  "fp",     cmd_fp,     " path // full path testing",
  // "ls0",    cmd_ls0,    "      // dummy list",
  "df",     cmd_df,     "      // disk infomation",
  "ls",     cmd_ls,     "path  // list directory",
  "mkdir",  cmd_mkdir,  "path  //make directory ",
  "rmdir",  cmd_rmdir,  "path  // remove directory",
  "cat",    cmd_cat,    "file  //diplay file content",
  "rm",     cmd_rm,     "file  // remove file",
  "echo",   cmd_echo,   "msg   //echo message",
  "write",  cmd_write,  "file message //" ,
  "append", cmd_append, "file message //",
  "cp",     cmd_cp,     "file1 file2 // copy file1 to file2",
  "mv",     cmd_mv,     "file1 file2. // rename file1 to file2",
  "time",   cmd_time,   "time",
  "pio",    cmd_pio,    "set LED pin output value",
  "dl",     cmd_download,"dl file  // download file to PC",
  "status", cmd_status,  "status",
  "TinyBasic",  cmd_tinybasic,  " // tinyBasic",
  "tb",     cmd_tinybasic,  " // tinyBasic",
  "ping",   cmd_ping,  " // ping test",
  "cls",    cmd_cls,  "  // clear screen",
  "help",   cmd_help,   " //help all command is case sensitive now!",
  "h",      cmd_help,   "help",
  "?",      cmd_help,   "help",
 //  "%",      cmd_prompt,   "//prompt",
 "task",   cmd_task,  "  // tasktest testing...",
 "dump",  cmd_dumpHex,   "0xaddr len //  dump Physical memory testing...",
 "call",   cmd_call,     "address argv ...// call cmd function  testing...",
   "",      cmd_prompt,   "",
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
    int  n;
    strcpy (argvline, str.c_str ());
    *p++ = strtok (argvline, " \n");
    for (n = 1; NULL != (*p++ = strtok (NULL, " \n")); n++)
        if (size == n)
            break;
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

  for(i=0;i<CMDNUMBERS;i++){
      if (commandTable[i].funName == "" || argc == 0) break;
      if (String(argv[0]) == commandTable[i].funName) {
         InterpreterCmdIndex = i;
         wsTextPrint("\n"); // newline for console
         Serial.printf("Command:%s found!",argv[0]);
         int ret=(*(commandTable[i].cmdPtr))(argc,argv);
          *cmd = "";


        //  Serial.printf(" %d\n",ret);
         return ret;
      // }else{
      //     cmd_prompt(argc,argv);
      }
  } 
  *cmd = "";
  if (argc>0) wsTextPrint("illegal cmd, try help");
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
           wsTextPrintln("ESP32 Websocket TinyConsole (Author:spuggy0919) " +  HTTP_CONSOLE_Version);
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

bool option(char *argv){
  InterpreterCmdOption = 0;
   if (argv==NULL) return false;

   char c=argv[0];
   char *p=&argv[1];
   if (c!='-') return false;
   while(*p!=0){
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
    }
    wsTextPrintln("In TinyBasic, Running break press '#' \n");
    wsTextPrintln("Exit TinyBasic,  press 'CTRL-C' \n");
    return 0;
}
int cmd_time(int argc,char * argv[]){
    wsTextPrintln(timerCurrent());
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
int cmd_df(int argc,char * argv[]){
  wsTextPrintln(reportfs(LittleFS).c_str());
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
int cmd_status(int argc,char * argv[]){
     return interpreterState;

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
    wsTextPrint("\x1b[2J");
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


#define __INTEPRETER_H__
#ifdef __INTEPRETER_H__
#define  HTTP_CONSOLE_Version (String("Version 0.2"))
#include "ESP32_8266.h"
#include <Base64.h>
#include "TinybasicArduino.h"
// #include "TinybasicPlusPort.h"

#include "dumphex.h"
#include "testmain.h"

// command option TODO 
#define COMMAND_OPTION(x) (((unsigned int)1)<<(x))
#define COMMAND_OPTION_EXIST         COMMAND_OPTION(15)
#define COMMAND_RECURIVE_DIRECTORY   COMMAND_OPTION(0)
#define COMMAND_FILE_WILDCHAR_FOUND  COMMAND_OPTION(1)
#define COMMAND_ARG1_PATH_FOUND      COMMAND_OPTION(2)
#define COMMAND_ARG2_PATH_FOUND      COMMAND_OPTION(3)
#define COMMAND_ARG1_WILDCARD_FOUND  COMMAND_OPTION(4)
#define COMMAND_ARG2_WILDCARD_FOUND  COMMAND_OPTION(4)



void parsercmdline(String line);
void wsTextPrintln(String msg);
void wsTextPrint(String msg);
void eventDownloadFile(String msg);


void interpreterInit();
int InterpreterExcute(String *cmd);

bool interpreterCheckReceiveCmd(String cmd);

int interpreter();

// main_cmd list
int cmd_help(int argc, char *argv[]);
int cmd_pio(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmd_pwd(int argc, char *argv[]);
int cmd_test(int argc, char *argv[]);
int cmd_task(int argc, char *argv[]);
int cmd_exec(int argc, char *argv[]);
int cmd_kill(int argc, char *argv[]);
int cmd_ps(int argc, char *argv[]);

#endif 
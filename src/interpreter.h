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
#define __INTEPRETER_H__
#ifdef __INTEPRETER_H__
#define  HTTP_CONSOLE_Version (String("Version 0.2"))
#include "ESP32inc.h"
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

#include "command.h"

#endif 
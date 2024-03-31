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

#include "version.h"
#define TINYCONSOLE _TC_VERSION_(TINYCONSOLE_API_MAJOR_VERSION,TINYCONSOLE_API_MINOR_VERSION,TINYCONSOLE_API_PATCH_VERSION)
#define TINYCONSOLEVSTR _TC_VERSION_STR_(TINYCONSOLE_API_MAJOR_VERSION,TINYCONSOLE_API_MINOR_VERSION,TINYCONSOLE_API_PATCH_VERSION)

String BannerOfTC = " \
           _____     _              _  _                 \n \
    o O O |_   _|   (_)    _ _     | || |                \n \
   o        | |     | |   | ' \\     \\_, |                \n \
  TC__[O]  _|_|_   _|_|_  |_||_|   _|__/                 \n \
 {======|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|                \n \
./o--000'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'                \n \
   ___                                      _            \n \
  / __|    ___    _ _      ___     ___     | |     ___   \n \
 | (__    / _ \\  | ' \\    (_-<    / _ \\    | |    / -_)  \n \
  \\___|   \\___/  |_||_|   /__/_   \\___/   _|_|_   \\___|  \n \
_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"| \n \
\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-'\"\n \
";

#ifdef  _LICENSE_BANNER_NOTICE_

String BannerTinyConsoleCopyRightNotice =" \
ESP32 Websocket TinyConsole\n"+TINYCONSOLEVSTR+" C2022/10/19 \n \
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

String BannerMessageWithJerryScriptCopyRightNotice = "\n \
JerryScript License \n \
 Copyright JS Foundation and other contributors, http://js.foundation\n \
\n \
 Licensed under the Apache License, Version 2.0 (the \"License\");\n \
 you may not use this file except in compliance with the License.\n \
 You may obtain a copy of the License at\n \
\n \
     http://www.apache.org/licenses/LICENSE-2.0\n \
\n \
 Unless required by applicable law or agreed to in writing, software\n \
 distributed under the License is distributed on an \"AS IS\" BASIS\n \
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n \
 See the License for the specific language governing permissions and\n \
 limitations under the License.\n \
\n";


void Banner_Tc(){
        wsTextPrintf(BannerOfTC.c_str()); // prompt
        wsSerial.getChar();
}
void tc_Prompt(){
        wsSerial.write("\n%%",2); // prompt
}
void tc_Banner(){

        cls();
#ifdef TINYBASIC
        wsTextPrintf(BannerTinyConsoleCopyRightNotice.cstr());
        wsTextPrintf("Buildin TinyBasic with below");
        wsTextPrintf(BannerMessageWithTinyBasicCopyRightNotice.c_str());


#endif
#ifdef _LANG_JERRYSCRIPT_
        wsTextPrintf(BannerMessageWithJerryScriptCopyRightNotice.c_str());

#endif
        // wsSerial.getChar();
        Banner_Tc();
        wsTextPrintf("\x1b[2J"); // cls
        wsTextPrintf("tinyConsole %s\n",TINYCONSOLEVSTR.c_str());
        wsTextPrintf("Copyright (c) 2022-2024 spuggy0919\n");
        tc_Prompt();
}

#else
#include "version.h"

void Banner_Tc(){
        wsTextPrintf(BannerOfTC.c_str()); // prompt
        wsSerial.getChar();
}
void tc_Prompt(){
        wsSerial.write("\n%%",2); // prompt
}
void tc_Banner(){
        Banner_Tc();
        tc_Prompt();
}
#endif
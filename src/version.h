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

#ifndef _LICENSE_H__
#define _LICENSE_H__
#include "cmdconfig.h"
#include "ESP32inc.h"


#define TINYCONSOLE_API_MAJOR_VERSION 0
#define TINYCONSOLE_API_MINOR_VERSION 9
#define TINYCONSOLE_API_PATCH_VERSION 1

#define _TC_VERSION_(major,minor,patch) (major*1000)+((minor%10)*100)+(patch%100)
#define _TC_VERSION_STR_(major,minor,patch) (String(major)+"."+String(minor%10)+"."+String(patch%100))

void tc_Prompt();
void tc_Banner();
#define cls()  wsTextPrintf("\n%")
#endif //_LICENSE_H__
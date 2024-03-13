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
#include "ESP32inc.h"

void cmd_example_setup() {
    /* Wifi, WebServer, websocket fs keyNotHit graphics functions is ready*/
        wsTextPrintf("cmd_setup here\n"); // websocket print to console
}
int cmd_example(int argc,char *argv[]){
    cmd_example_setup();
    while(!wsSerial.escape()){ // loop
        wsTextPrintf("cmd_loop here\n"); // websocket print to console
        delay(100);
    }
    return 0;
}
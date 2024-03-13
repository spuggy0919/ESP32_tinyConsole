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
#include "plotLabel.h"

/* include your librar and add to ini library depe*/
plotLabel HelloLabel("您好！Hello World!");


void cmd_hello_setup(){
    HelloLabel.at(0.04,0.6);     // whole screen from (0,0) to (1.0,1.0) 
    HelloLabel.fontSize(100);   // 100px
    HelloLabel.fontColor(0xff00ffff,0x000000FF); //RGBA
    HelloLabel.plot();
}
float _x =0.04;
bool cmd_hello_loop(){

    if (wsSerial.escape()) return false;

    /* loop do somthing*/
    _x-=0.05; if (_x<-0.8) _x=1.0;
    HelloLabel.at(_x,0.6);     // whole screen from (0,0) to (1.0,1.0) 
    HelloLabel.plot();
    wsTextPrintf("Hello World\n"); // websocket print to console


    delay(200);

    return true; // true, forever
}



/* don't need to change below, ignore this */

int cmd_hello(int argc,char *argv[]){
    cmd_hello_setup();
    while(cmd_hello_loop()); 
    return 0;
}
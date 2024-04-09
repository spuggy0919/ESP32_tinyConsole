/*
 * This file is part of ESP32_TinyConsole.
 *
 *  Copyright (c) 2024 spuggy0919
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
/*
 * @file  1sio.js
 * @brief use sio demo
 * 
 * @details 
 *          kbhit
 *          getline
 *          writeline
 * @module id sio
 * @instance wsSerial
 * 
 * @date Apirl 3, 2024
 */
let sio = require('sio');
function kbhit(){ // by esc, control-c, q, Q
    let b = false;
    while(!b) {
        b=sio.escape();
    }
    return b;
}
function getc(){ // now wsSerial is sent line base, should be enter then sent out
    c=sio.getchar();
    return c;
}

function getline(){
    s=sio.readline();
    return s;
}

function writeline(s){ //wsSerial.write write string to ws tx queue buffer
    return sio.writestring(s);
}

console.log("Hit by key testing(esc,ctrl-c,'q','Q')...\n");
console.log(kbhit());
console.log("getline...");
str = getline();
console.log('\n',str,"\n");
console.log("writeline...");
writeline("中文ABC測試\n");
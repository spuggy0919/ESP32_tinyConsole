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
 *
 */
/*
 * @file  index.js
 * @brief autorun example  
 * 
 * @details
 *  when esp32 restart, the main.cpp will check autorun config
 *  1. if config.json define autorun key, it is first priority
 *  2. check /js/index.js for jerryscript
 *     check /basic/autoexec.bas for tinybasic
 *     if file exist, then run it
 *  3. if autorun exit, or non autorun mode, the default shell interpreter will run.
 * 
 * @classname none
 * @instance 
 * 
 * @date Apirl 3, 2024
 */
let fs = require('fs');
let sio = require('sio');
let touch = require('touch');
let wifi = require('wifi');
let process = require('process');



function getuser(process){
    process.usrInput = (sio.readline()).slice(0,-1); // delete linefeed
}
function getpass(process){
    process.pwdInput = (sio.readline()).slice(0,-1); // delete linefeed
}
function IsMatch(process){
    let ret = false;
    ret = (process.user !== '' ) ;
    ret |=(process.user !== 'anonymous' );
    ret |= process.usrInput === 'super'; // funny
    ret |= process.usrInput === '' || process.pwdInput === ''; // dummy
    ret |= ((process.usrInput === process.user)&&(process.pwdInput === process.passwrd)); // actual
    return(ret);
}
function Authorize(process){
    // console.log(typeof process.user, process.user);
        sio.flush();
    while(1) {
        if (IsMatch(process)) break;
        sio.print('\nusername:');getuser(process); //sio.readline();
        sio.print('\npassword:');getpass(process); //sio.readline();
    }
    sio.print('\n');
}
let waitcnt = 0; 
while(!touch.attach()){ // websocket running touch is enable.
    waitcnt+=1;
    if (sio.escape()) break; // websocket running sio is enable.
}; // if interactive, wait for websocket connect
sio.print("\x1b[2J"); 
delay(500); //wait for cls
sio.print("\n\n\n\n\n\n\n\n\n\n");
sio.print("\n\n\n\n\n\n\n\n\n\n"); // move cursor down
sio.print("\ntinyConsole \n");
Authorize(process);
if (wifi.getmode() == wifi.WIFI_STA) { 
    console.log("WIFI_STA",wifi.ssid());
    console.log("hostIP  ",wifi.localIP());
    console.log("clientIP",wifi.clientIP());
    console.log("status",wifi.status());
    fs.cd('/js/');
}else{
    console.log("WIFI_AP");
    console.log("config your ssid,password");
    console.log("%export ssid ABCD [enter]");
    console.log("%export password 12345678 [enter]");
}
console.log("Autorun cmd arglist");
console.log("cwd=",process.cwd());
console.log("argc=",process.argv.length);
let i=0;
process.argv.forEach(function(element){
    console.log(`argv[${i}]`,element);     i++;
});
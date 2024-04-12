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
 * @file  dumpg.js
 * @brief dump native objects
 * 
 * @details
 *  native objects are implement by C++ wrapper, and register by jswrap_tc
 * 
 * @classname none
 * @instance 
 * 
 * @date Apirl 3, 2024
 */
function listObjProps(obj){ //move to module.js
  // Enumerate and list all elements (including functions) in the object
  for (let key in obj) {
    if (obj.hasOwnProperty(key)) {
      console.log(key,"\t",obj[key]); // Print the property name
    }
  }
} 

let nativeobjs=[this,wsSerial,File,wifi,Touch,Canvas,dht,console];
nativeobjs.forEach(element => {
  // print(element);
  listObjProps(element);
});


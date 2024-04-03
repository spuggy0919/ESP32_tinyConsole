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
 * @file  0date.js
 * @brief Date() object
 * 
 * @details
 *  Date() object of JerryScript
 * 
 * @classname Date
 * @instance none
 * 
 * @date Apirl 3, 2024
 */
var now = new Date();  // Create a Date object representing the current date and time
console.log("Date =",now.getDate());         // Get the day of the month
console.log("Day  =",now.getDay());          // Get the day of the week
console.log("Year =",now.getFullYear());     // Get the year
console.log("Hours=",now.getHours());        // Get the hour
console.log("Mins =",now.getMinutes());      // Get the minutes
console.log("Secs =",now.getSeconds());      // Get the seconds
console.log("Milli=",now.getMilliseconds()); // Get the milliseconds
console.log("Time =",now.getTime());          // Get the number of milliseconds since the Unix epoch
console.log("toStr=",now.toString());         // Get a string representation of the Date object


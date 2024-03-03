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
/*
 * @file touch.cpp
 * @brief  touch queue and mutex
 * 
 * @details
 *  when user trigger the touch then send coordinate to touch panel queue here
 * 
 * @date March 1, 2024
 */

/**
 * @brief touch input queue
 * 
 * @details
 * This function queue  touch events.
 * 
 */
#include "touch.h"
TouchQueue gTouchQueue;

TouchQueue::TouchQueue() {
    mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(mutex);
}

TouchQueue::~TouchQueue() {
    vSemaphoreDelete(mutex);
}

bool  TouchQueue::flush() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        touchList.clear();
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

size_t TouchQueue::available() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        size_t result = touchList.size();
        xSemaphoreGive(mutex);
        return result;
    }
    return 0;
}

void TouchQueue::push(String msg) {  //T:X:Y
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {

        if (touchList.size()>TOUCHQUEUESIZE) touchList.pop_front();
        // Use indexOf to find the positions of colons
        int firstColon = msg.indexOf(':');
        int secondColon = msg.indexOf(':', firstColon + 1);

        // Extract substrings between colons
        String part1 = msg.substring(0, firstColon);
        String part2 = msg.substring(firstColon + 1, secondColon);
        String part3 = msg.substring(secondColon + 1);

        // Convert substrings to integers
        int event = part1.toInt();
        int x = part2.toInt();
        int y = part3.toInt();

        // Now, num1, num2, and num3 contain the integer values
        // parser to touch x, y
        touchList.push_back(std::make_tuple(event, x, y));
    // Serial.printf("TouchQueue::push(%d,%d,%d)\n",event,x,y);

        xSemaphoreGive(mutex);
    }
}
void TouchQueue::push(int event, int x, int y) {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        if (touchList.size()>TOUCHQUEUESIZE) touchList.pop_front();
        touchList.push_back(std::make_tuple(event, x, y));
        xSemaphoreGive(mutex);
    }
}

bool TouchQueue::pop(int &event, int &x, int &y) {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        if (!touchList.empty()) {
            std::tuple<int, int, int> front = touchList.front();
            event = std::get<0>(front);
            x = std::get<1>(front);
            y = std::get<2>(front);
            touchList.pop_front();
            xSemaphoreGive(mutex);
            // Serial.printf("TouchQueue::pop(%d,%d,%d)\n",event,x,y);
            return true;
        }
        xSemaphoreGive(mutex);
    }
    return false;
}
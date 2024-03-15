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
#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <Arduino.h>
#include <list>
#include <mutex>

class TouchQueue {
public:
    TouchQueue();
    ~TouchQueue();

    bool flush();
    size_t available();
    void push(String msg);
    void push(int event, int x, int y);
    bool pop(int &event, int &x, int &y);

private:
    std::mutex touchMutex;
    std::list<std::tuple<int, int, int>> touchList;
};
#define TOUCHQUEUESIZE 16
extern TouchQueue gTouchQueue;

#endif //_TOUCH_H_
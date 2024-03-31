/*
 * This file is part of ESP32_TinyConsole.
 *   Copyright (c) 2024 Spuggy0919, spuggy0919@gmail.com
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
 * @file wsTask.h
 * @brief  websocker task using Ticker for arduino platform
 * 
 * @details
 * User can queue a task and Execute by ticker with SetInterval, SetTimeout , delete , queue function
 * @date March 19, 2024
 */
#ifndef _WSTASK_H_
#define _WSTASK_H_
#include "cmdconfig.h"
#ifdef _TICKER_TASK_
#include <Arduino.h>
#include <Ticker.h>
#include <mutex>
#include <queue>

// Task status enum
enum class TaskStatus {
    WAIT,
    READY,
    RUNNING,
    PENDDING,
    STOPPED
};
enum class TaskType{
    RUNLOOP,
    RUNINTERVAL,
    RUNTIMEOUT,
    STOPPED
};
// Task structure
class Task{
    public:
        Task(){

        }
    String          name;
    TaskType        taskType;
    int             ret;
    unsigned long   interval;
    unsigned long   lastExecution;
    TaskStatus      status;
    MAINPTR         function;
};

typedef void xTaskFuc(void *pvParameters);

Class wsTaskMgr{
public:
    wsTaskMgr();
    ~wsTaskMgr();
    void queueTaskSetInterval(const String& name, unsigned long interval, MAINPTR function);
    void queueTaskClearInterval(const String& name);
    void queueTaskSetTimeout(const String& name, unsigned long Timeout, MAINPTR function);
    void queueTaskRunLoop(const String& name, MAINPTR function);
    String reportStatus();
private:
    std::queue<Task> tasks;
    std::mutex mutex;
    Ticker ticker;

}
#endif
#endif
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
 * @file wsTask.cpp
 * @brief  websocker task using Ticker for arduino platform, ticker does not inderently yield 
 * control automatically. Task is needed to limit its runtime. 
 * If possible change to freertos version, this version is for non freertos platform.
 * 
 * @details
 * User can queue a task and Execute by ticker with SetInterval, SetTimeout , delete , queue function
 * @date March 19, 2024
 */
#include "cmdconfig.h"

##ifdef  FREERTOS_TIMERTASK
#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue>

std::mutex mutex;
std::queue<TaskHandle_t> taskQueue;

void taskFunction(void *pvParameters) {
    for (;;) {
        // Task execution logic
        vTaskDelay(pdMS_TO_TICKS(1000)); // Example delay of 1 second
    }
}

class TaskManager {
public:
    TaskManager() {
        // Initialize mutex if needed
        // Initialize task queue if needed
    }

    ~TaskManager() {
        // Cleanup tasks and resources on destruction
    }

        // Task that simulates a periodic function call (setInterval equivalent)
typedef void callback_t();
struct timeInterval{
    TickType_t tick;
    callback_t   func;
};
typedef struct timeInterval *pTimer_t;

    void periodicFunction(void* parameters) {
        pTimer_t p = (pTimer_t)parameters;
        TickType_t interval = pdMS_TO_TICKS(p->ticks);
        callback_t func = p->func;
        while (1) {
            // Perform the periodic operation here
            func();
            vTaskDelay(interval);
        }
    }
    // Task that simulates a delayed function call (setTimeout equivalent)
    void delayedFunction(void* parameters) {
        pTimer_t p = (pTimer_t)parameters;
        TickType_t interval = pdMS_TO_TICKS(p->ticks);
        vTaskDelay((*(TickType_t*)parameters) / portTICK_PERIOD_MS);
        // Perform the delayed operation here
        callback_t func = p->func;

    }
    void setTimeout(unsigned int delayMs,callback_t callfunc) {
            // Calculate the interval in ticks
            TickType_t ticks = pdMS_TO_TICKS(intervalMs);
            pTimer_t param ={pdMS_TO_TICKS(intervalMs),callfunc};

        // Create a task to simulate the delayed function call
        xTaskCreate(delayedFunction, "DelayedFunction", configMINIMAL_STACK_SIZE, &ticks, tskIDLE_PRIORITY + 1, NULL);
    }
    void setInterval(unsigned int intervalMs,callback_t callfunc) {
        if (intervalTaskHandle == NULL) {
            // Calculate the interval in ticks
            TickType_t ticks = pdMS_TO_TICKS(intervalMs);
            pTimer_t param ={pdMS_TO_TICKS(intervalMs),callfunc};
            // Create a task for the periodic function call
            xTaskCreate(periodicFunction, "PeriodicFunction", configMINIMAL_STACK_SIZE, param, tskIDLE_PRIORITY + 1, &intervalTaskHandle);
        }
    }

    void clearInterval() {
        if (intervalTaskHandle != NULL) {
            vTaskDelete(intervalTaskHandle);
            intervalTaskHandle = NULL;
        }
    }

    String reportStatus() {
        char taskListBuffer[512]; // Buffer to hold the task list information
        // Generate the task list information
        vTaskList(taskListBuffer);
        return String(taskListBuffer);
    }

    void begin() {
        // Start the task manager if needed
    }

    void end() {
        // End the task manager and cleanup resources if needed
    }
};

TaskManager taskManager;

void setup() {
    Serial.begin(9600);

    // Queue tasks
    taskManager.queueTask("Task1", 1000); // Queue task1 to run every 1 second
    taskManager.queueTaskOnce("Task2", 5000); // Queue task2 to run once after 5 seconds

    // Start task manager
    taskManager.begin();
}

void loop() {
    // Your main loop code here
}

#endif
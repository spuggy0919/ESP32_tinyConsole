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
#ifdef _TICKER_TASK_
#include <Arduino.h>
#include <queue>
#include <mutex>
#include <Ticker.h>

    wsTaskMgr::wsTaskMgr() {
        // Initialize ticker to call tick function every 1 second
        ticker.attach(1, std::bind(&wsTaskMgr::tick, this));
        ticker.begin();
    }

    ~wsTaskMgr::wsTaskMgr() {
        // Stop the ticker on destruction
        ticker.detach();
        ticker.stop();
    }

    void wsTaskMgr::queueTaskInterval(const String& name, unsigned long interval, MAINPTR function) {
        Task task = {name, TaskType::RUNINTERVAL, interval, 0, TaskStatus::READY, function};
        mutex.lock();
        tasks.push(task);
        mutex.unlock();
    }

    void wsTaskMgr::queueTaskOnce(const String& name, unsigned long delay, MAINPTR function) {
        Task task = {name, TaskType::RUNTIMEOUT, delay, 0, TaskStatus::WAIT, function};
        mutex.lock();
        tasks.push(task);
        mutex.unlock();
    }

    void wsTaskMgr::startTask(Task& task) {
        task.status = TaskStatus::RUNNING;
        task.lastExecution = millis();
        task.ret = task.function(task.argc,task.argv);
    }

    void wsTaskMgr::stopTask(Task& task) {
        task.status = TaskStatus::STOPPED;
    }

    void wsTaskMgr::removeTask(const String& name) {
        mutex.lock();
        std::queue<Task> temp;
        while (!tasks.empty()) {
            Task task = tasks.front();
            tasks.pop();
            if (task.name != name) {
                temp.push(task);
            }
        }
        tasks = temp;
        mutex.unlock();
    }

    void tick() {
        unsigned long currentTime = millis();
        mutex.lock();
        while (!tasks.empty()) {
            Task& task = tasks.front();
            switch(task.taskType){
                case taskType::RUNLOOP:
                    if (task.status!=taskStatus::STOPPED) startTask(task);
                break;
                case taskType::RUNINTERVAL:
                if (task.lastExecution==0||(millis()-task.lastExecution)>task.interval){
                    startTask(task);
                }else{
                    task.status = taskStatus::READY;
                }
                break;
                case taskType::RUNTIMEOUT:
                if (task.status==WAIT){
                    if (task.lastExecution==0) task.lastExecution=millis();
                    if ((millis()-task.lastExecution)>task.interval) {
                        startTask(task);
                    }
                }else if (task.status==taskStatus::READY||task.status==taskStatus::RUNNING){
                    task.status=taskStatus::STOPPED;
                }else{
                    task.status=taskStatus::STOPPED;
                }         
                break;
                case taskType::STOPPED:
                break;
            }
            if (task.status == TaskStatus::READY) {
                task.status = TaskStatus::RUNNING;
                task.lastExecution = currentTime;
                task.function();
            } else if (task.status == TaskStatus::RUNNING && currentTime - task.lastExecution >= task.interval) {
                task.lastExecution = currentTime;
                task.function();
            }
            if (task.status == TaskStatus::STOPPED) {
                tasks.pop();
            } else {
                break;
            }
        }
        mutex.unlock();
        yield();
    }

    String reportStatus() {
        String report;
        mutex.lock();
        std::queue<Task> tempQueue = taskQueue; // Create a copy of the queue for iteration
        mutex.unlock();
        while (!tempQueue.empty()) {
            Task task = tempQueue.front();
            report+=task.name;
            report+="\t"+task.TaskType;
            Serial.print(", Status: ");
            switch (task.status) {
                case TaskStatus::WAIT:
                    report+="\tWAIT";
                    break;
                case TaskStatus::READY:
                    report+="\tREADY";
                    break;
                case TaskStatus::RUNNING:
                    report+="\tRUNNING";
                    break;
                case TaskStatus::PENDING:
                    report+="\tPENDING";
                    break;                
                case TaskStatus::STOPPED:
                    report+="\tSTOPPED";
                    break;
            }
            report+="\n";
            tempQueue.pop();
        }
    }

    void begin() {
        ticker.start();
    }

    void end() {
        ticker.stop();
    }
};

TaskManager taskManager;

// Example tasks
void task1() {
    Serial.println("Task 1 executed");
}

void task2() {
    Serial.println("Task 2 executed");
}

// void setup() {
//     Serial.begin(9600);

//     // Queue tasks
//     taskManager.queueTask("Task1", 1000, task1); // Queue task1 to run every 1 second
//     taskManager.queueTaskOnce("Task2", 5000, task2); // Queue task2 to run once after 5 seconds

//     // Start task manager
//     taskManager.begin();
// }

// void loop() {
//     // Your main loop code here
// }
#else // __FREERTOS_YASK__
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

    void queueTask(const String& name, unsigned long interval) {
        TaskHandle_t taskHandle;
        xTaskCreate(taskFunction, name.c_str(), 2048, NULL, 1, &taskHandle);
        mutex.lock();
        taskQueue.push(taskHandle);
        mutex.unlock();
    }

    void queueTaskOnce(const String& name, unsigned long delay) {
        TaskHandle_t taskHandle;
        xTaskCreate(taskFunction, name.c_str(), 2048, NULL, 1, &taskHandle);
        mutex.lock();
        taskQueue.push(taskHandle);
        mutex.unlock();
    }

    void startTask(TaskHandle_t taskHandle) {
        vTaskResume(taskHandle);
    }

    void stopTask(TaskHandle_t taskHandle) {
        vTaskSuspend(taskHandle);
    }

    void removeTask(const String& name) {
        mutex.lock();
        std::queue<TaskHandle_t> temp;
        while (!taskQueue.empty()) {
            TaskHandle_t taskHandle = taskQueue.front();
            taskQueue.pop();
            if (String(pcTaskGetTaskName(taskHandle)) != name) {
                temp.push(taskHandle);
            } else {
                vTaskDelete(taskHandle); // Delete the task
            }
        }
        taskQueue = temp;
        mutex.unlock();
    }

    void reportStatus() {
        mutex.lock();
        while (!taskQueue.empty()) {
            TaskHandle_t taskHandle = taskQueue.front();
            Serial.print("Task: ");
            Serial.print(pcTaskGetTaskName(taskHandle));
            Serial.print(", Status: ");
            eTaskState taskState = eTaskGetState(taskHandle);
            switch (taskState) {
                case eReady:
                    Serial.println("READY");
                    break;
                case eRunning:
                    Serial.println("RUNNING");
                    break;
                case eSuspended:
                    Serial.println("SUSPENDED");
                    break;
                case eDeleted:
                    Serial.println("DELETED");
                    break;
                case eBlocked:
                    Serial.println("BLOCKED");
                    break;
                case eInvalid:
                    Serial.println("INVALID");
                    break;
            }
            taskQueue.pop();
        }
        mutex.unlock();
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
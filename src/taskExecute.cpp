
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
 * @file excute.c
 * @brief This file contains the implementation of the excute cmmline functon on background.
 * 
 * @details
 * The functions create a task and run until killed.
 * 
 * @date February 28, 2024
 */

/**
 * @brief Add two integers.
 * 
 * @details
 * This function takes two integers as input and returns their sum.
 * 
 * @param[in] num1 The first integer to be added.
 * @param[in] num2 The second integer to be added.
 * @return The sum of num1 and num2.
 */
// int addIntegers(int num1, int num2) {
//     return num1 + num2;
// }
#include "ESP32INC.h"
#include "interpreter.h"
#include "taskExecute.h"

  SemaphoreHandle_t TaskExcuteWrapper::cmdMux=xSemaphoreCreateMutex();

  TaskExcuteWrapper::TaskExcuteWrapper(const char* name, int stackSize, UBaseType_t priority) {
    // Initialize variables
    taskName = name;
    stackSizeBytes = stackSize;
    taskPriority = priority;
  }

  TaskExcuteWrapper::~TaskExcuteWrapper() {
    // Destructor, make sure to delete the task if it exists
    if (taskHandle != NULL) {
      vTaskDelete(taskHandle);
    }
  }

  void TaskExcuteWrapper::startTask() {
    // Create the task
    xTaskCreate(
      taskFunction, // Task function
      taskName,                  // Task name
      stackSizeBytes,            // Stack size
      this,                      // Task parameter (pointer to the instance)
      taskPriority,              // Task priority
      &taskHandle                // Task handle
    );
    taskState =  String("cmdTask Running");
  }

  void TaskExcuteWrapper::stopTask() {
    // Delete the task if it exists
    if (taskHandle != NULL) {
      vTaskDelete(taskHandle);
      // xSemaphoreGive(cmdMux);
      taskHandle = NULL;
    taskState =  String("cmdTask Stop");

    }
  }

  void TaskExcuteWrapper::sendCommand(const char* command) {
    // Push the command to the command queue
    cmdStr = String(command);
    xQueueSendToBack(commandQueue, command, portMAX_DELAY);
    queueList.push_back(cmdStr);
    taskState =  String("Ready Queue");
  }
  void TaskExcuteWrapper::giveMutex() {
            xSemaphoreGive(cmdMux);
  }
  String TaskExcuteWrapper::listAll() {
   String allElements;
   for (const auto& element : queueList) {
        // Handle each element, e.g., print to Serial
        allElements+="\t"+element + "\n";
        // Serial.println(element);
    }
    return allElements;
  }
  TaskStatus TaskExcuteWrapper::getStatus() {
    TaskStatus status;
    status.handle = taskHandle;
    status.name = taskName;
    status.priority = taskPriority;
    status.stackSize = stackSizeBytes;
    status.stackFree = uxTaskGetStackHighWaterMark(taskHandle);
    status.cmd = listAll();
    status.state = taskState;
    return status;
  }
  void TaskExcuteWrapper::taskFunction(void* obj) {
    static_cast<TaskExcuteWrapper*>(obj)->runTask(); // Call the member function
  }
  // static void TaskExcuteWrapper::taskFunction(void* obj) {
  //   // Task function (static), calls the member function
  //   static_cast<TaskWrapObj*>(obj)->runTask(); 
  //       // vTaskDelete(NULL);

  // }

  void TaskExcuteWrapper::runTask() {
    // Task logic
    char commandBuffer[32]; // Adjust the size as needed
    String cmd;
    while (1) {
      // Check for commands in the queue
      taskState =  String("Wait Cmd");
      if (xQueueReceive(commandQueue, commandBuffer, portMAX_DELAY)) {
        // Process the command (example: print to Serial)
        // queueList.pop_front();
        Serial.print("Received command: ");
        Serial.println(commandBuffer);
        taskState =  String("wait Mutex");
        while(xSemaphoreTake(cmdMux, portMAX_DELAY) == pdFALSE) {
            vTaskDelay(pdMS_TO_TICKS(500)); // Example: 1-second delay
        }
        Serial.printf("get new commandline%s running\n",commandBuffer);
        taskState =  String("Running Cmd");
        cmd=String(commandBuffer);
        InterpreterExcute(&cmd);
        
        xSemaphoreGive(cmdMux);
      }

      // Add your task logic here

      // Use vTaskDelay or other FreeRTOS functions for task timing
      vTaskDelay(pdMS_TO_TICKS(1000)); // Example: 1-second delay
    }
  }

// TaskExcuteWrapper instance
TaskExcuteWrapper taskExec("cmdTask",0x2000,tskIDLE_PRIORITY);

int cmd_exec(int argc, char *argv[]){
// exec cmdline 
    if (argc<2) return -1; // error 
    if (argv[1][0]=='G'||argv[1][0]=='g'){
        taskExec.giveMutex();
        return 0;
    }
    taskExec.startTask();
    taskExec.sendCommand(String(argv[1]).c_str());
    return 0;
}


int cmd_kill(int argc, char *argv[]){
// exec cmdline 
    taskExec.stopTask();
    return 0;
}


int cmd_ps(int argc, char *argv[]){
// exec cmdline 
TaskStatus status = taskExec.getStatus();
       wsTextPrintf("taskName:  %s\n",status.name);
       wsTextPrintf("taskcmd:   %s\n",status.cmd.c_str());
       wsTextPrintf("priority:  %d\n",status.priority);
       wsTextPrintf("taskState: %s\n",status.state.c_str());
       wsTextPrintf("stackSize: %d\n",status.stackSize);
       wsTextPrintf("stackFree: %d\n",status.stackFree);
    return 0;
}







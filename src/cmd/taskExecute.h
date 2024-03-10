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
#ifndef __TASK_EXECUTE__
#define __TASK_EXECUTE__

// Structure to hold task status
struct TaskStatus {
  TaskHandle_t handle;
  const char* name;
  UBaseType_t priority;
  uint16_t stackSize;
  uint16_t stackFree;
  String  cmd;
  String  state;

};
#include <list>

class TaskExcuteWrapper {
public: 
    TaskExcuteWrapper(const char* name, int stackSize, UBaseType_t priority);
    ~TaskExcuteWrapper();
    void startTask();
    void stopTask();
    void sendCommand(const char* command);
    TaskStatus getStatus();
    void giveMutex();
private:
  const char* taskName;
  std::list<String> queueList;
  String cmdStr = "";
  String taskState = "";
  int stackSizeBytes;
  UBaseType_t taskPriority;
  TaskHandle_t taskHandle = NULL;
  QueueHandle_t commandQueue = xQueueCreate(5, sizeof(char) * 32); // Adjust the size as needed
  static SemaphoreHandle_t cmdMux;
  static void taskFunction(void* obj);
  void runTask();
  String listAll();
};


int cmd_exec(int argc, char *argv[]);
int cmd_kill(int argc, char *argv[]);

#endif // __TASK_EXECUTE__
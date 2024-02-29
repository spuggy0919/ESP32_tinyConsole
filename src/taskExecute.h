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
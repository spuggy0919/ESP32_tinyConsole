#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <Arduino.h>
#include <Freertos/FreeRTOS.h>
#include <list>

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
    SemaphoreHandle_t mutex;
    std::list<std::tuple<int, int, int>> touchList;
};
#define TOUCHQUEUESIZE 16
extern TouchQueue gTouchQueue;

#endif //_TOUCH_H_
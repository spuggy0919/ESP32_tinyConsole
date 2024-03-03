#ifndef _ITOUCH_H_
#define _ITOUCH_H_

// #include <Arduino.h>
#include <Freertos/FreeRTOS.h>
#include "touch.h"

class ITouch {
public:
    ITouch();
    ~ITouch();
    void open();
    char checkch();
    unsigned short available();
    char inch();
    void ins(char *b, unsigned short nb) ;
private:
    String xy;
    int xyAt;
    String event;
    int eventAt;
    bool getNextTouch();
    void   flushTouch();
    char   buf[16];
};
extern TouchQueue gTouchQueue;
extern ITouch iTouch;

#endif //_ITOUCH_H_
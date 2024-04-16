#ifndef __LEDPWM__H__
#define __LEDPWM__H__
#include <Arduino.h>
//#define _PWM_IO_
#ifdef ESP32
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif
#else
#endif
// ESP32-CAM
// #define LED_ONBOARD 4
// #define LED_BUILTIN 4
extern bool pwmledChange;
extern String pwmledState;

void pwminit();
void pwmled(int dutyCycle);
int pwmledr();
bool pwmtrigger(int interval, int number, bool randflag);
int  pwmtriggernum();
int pwmtriggerstart();
int pwmtriggerstop();
bool pwmtriggerStopOrNot(); // true:stop, false:on going
String pwmlogData();

#endif // __LEDPWM__H__

#ifndef __LEDPWM__H__
#define __LEDPWM__H__
#include <Arduino.h>
//#define _PWM_IO_
#define LED_ONBOARD 2

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

#ifndef __TIMERFUN_H__ 
#define __TIMERFUN_H__ 
#include <Arduino.h>

#ifdef ESP32
#include <ESP32Time.h>
#include "ESPreset.h"
#include <sys/timeb.h>
extern ESP32Time rtc;
void timerSetTest();
String timerCurrent();
String timerGetEpoch();
String timerDate();
void timerSetEpoch(unsigned long epoch);
void getftime(struct timeb *tm);

#else
void timerSetTest();
String timerCurrent();
String timerGetEpoch();
void timerSetEpoch(unsigned long epoch);
extern ESP32Time rtc; // UTC+8
#endif

#endif // __TIMERFUN_H__ 
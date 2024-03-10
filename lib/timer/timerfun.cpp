/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "timerfun.h"
#include <sys/timeb.h>

#ifdef ESP32
ESP32Time rtc(3600*8); // UTC+8

void timerSetTest(){
    rtc.setTime(16, 13, 15, 4, 10, 2022);  // 4th Oct 2022 13:13:30
}
// time_t         time     The seconds portion of the current time. 
// unsigned short millitm  The milliseconds portion of the current time. 
// short          timezone The local timezone in minutes west of Greenwich. 
// short          dstflag  TRUE if Daylight S
void getftime(struct timeb *tm){
   tm->time = rtc.getSecond();
   tm->millitm = rtc.getMillis();
   tm->timezone = 4800;
   tm->dstflag = false;

}
String timerCurrent(){
    return rtc.getTime();          //  (String) 15:24:38
}
String timerDate(){
    return rtc.getDate();          //  (String) 15:24:38
}
String timerGetEpoch(){
    return String(rtc.getEpoch());          //  (String) 15:24:38
}
void timerSetEpoch(unsigned long epoch){
  //  if (ESPHW_powerloss()){
        rtc.setTime(epoch);
        Serial.println("ESP timer Set");
 //   }
}
#else
void timerSetTest(){}
String timerCurrent(){return "";}
String timerGetEpoch(){return "";}
void timerSetEpoch(unsigned long epoch){}
#endif
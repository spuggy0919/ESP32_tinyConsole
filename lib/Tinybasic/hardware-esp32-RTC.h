/*
 *
 * $Id: hardware-arduino.h,v 1.4 2022/08/15 18:08:56 stefan Exp stefan $
 *
 * Stefan's basic interpreter 
 *
 * Playing around with frugal programming. See the licence file on 
 * https://github.com/slviajero/tinybasic for copyright/left.
 *   (GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007)
 *
 * Author: Stefan Lenz, sl001@serverfabrik.de
 *
 * Hardware definition file coming with TinybasicArduino.ino aka basic.c
 */
 
/* 
 * esp32 Real Time clock code based on ESP32TIME library,
 * https://github.com/fbiego/ESP32Time.git
 * ESP32RTC RTC function 
 * 
 */
/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#define ESP32RTC 
#ifdef ESP32RTC
#include "timerfun.h"
//extern ESP32Time rtc;
char rtcstring[18] = { 0 }; 

char* rtcmkstr() {
	int cc = 1;
	short t;
	char ch;
	t=rtcget(2);
	rtcstring[cc++]=t/10+'0';
	rtcstring[cc++]=t%10+'0';
	rtcstring[cc++]=':';
	t=rtcread(1);
	rtcstring[cc++]=t/10+'0';
	rtcstring[cc++]=t%10+'0';
	rtcstring[cc++]=':';
	t=rtcread(0);
	rtcstring[cc++]=t/10+'0';
	rtcstring[cc++]=t%10+'0';
	rtcstring[cc++]='-';
	t=rtcread(3);
	if (t/10 > 0) rtcstring[cc++]=t/10+'0';
	rtcstring[cc++]=t%10+'0';
	rtcstring[cc++]='/';
	t=rtcread(4);
	if (t/10 > 0) rtcstring[cc++]=t/10+'0';
	rtcstring[cc++]=t%10+'0';
	rtcstring[cc++]='/';
	t=rtcread(5);
	if (t/10 > 0) rtcstring[cc++]=t/10+'0';
	rtcstring[cc++]=t%10+'0';
	rtcstring[cc]=0;
	rtcstring[0]=cc-1;
	Serial.printf("rtc.%s\n",rtcstring);
	return rtcstring;
}

short rtcread(char i) {
	switch (i) {
		case 0: 
			return rtc.getSecond();
		case 1:
			return rtc.getMinute();
		case 2:
			return rtc.getHour();
		case 3:
			// Serial.printf("rtc.getDay%d\n",rtc.getDay());

			return rtc.getDay();
		case 4:
			// Serial.printf("rtc.getMonth%d\n",rtc.getMonth());

			return rtc.getMonth()+1;
		case 5:
			// Serial.printf("rtc.getYear%d\n",rtc.getYear());

			return rtc.getYear()%100;
		case 6:
			return rtc.getDayofWeek();
		case 7:
			return  0;//rtc.getMillis();
		default:
			return 0;
	}
}

short rtcget(char i) {
	//rtc.refresh();
	return rtcread(i);
}

void rtcset(char i, short v) {
	uint8_t tv[7];
	char j;
//	rtc.refresh();
	for (j=0; j<7; j++) tv[j]=rtcread(j);
	tv[i]=v;
	rtc.setTime(tv[0], tv[1], tv[2], tv[3], tv[4], tv[5], tv[6]);
}
#endif
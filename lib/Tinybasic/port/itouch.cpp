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
 * @file itouch.cpp
 * @brief  itouch port for tinybasic , using char input device
 *         the port defines
 *        1. set Stream device definition,
 *          #define ITOUCH 3
 *          useage: INPUT &3, "X", X, "Y", Y 
 *              or INPUT &3, "Event", E, "X", X, "Y", Y (TBD)
 * 
 *        2. define below functions 
 *           checkch() check first char in th input stream
 *           available check the coming buffer is data available
 *           inch      input first char
 *           ins       input line until carriage return
 *        other porting method is special char '@' (TBD)
 *              SENSOR (TBD)
 *              Set c,d (TBD)
 *                
 * @details
 *  when user trigger the touch then send coordinate to touchqueue, then here converter to char stream
 * 
 * @date March 1, 2024
 */
#include "itouch.h"

ITouch::ITouch(){
    xy=String();
    event=String();
    xyAt=-1;
    eventAt=0;
}
ITouch::~ITouch(){

}
void ITouch::open(){
    // Serial.printf("ITouch::open\n");
    flushTouch();
}
char ITouch::checkch(){
    // Serial.printf("ITouch::checkch");
    if (xy==NULL||xy.length()==0||xyAt==-1) { // Null xy or old one is transferred 
        // try to get next touch
        if (getNextTouch()) {
            // Serial.printf("ITouch::checkch(%s)at %d",xy,xyAt);
            return xy.charAt(xyAt);
        }
    }else {
        return xy.charAt(xyAt);
    }
    return 0;
}
unsigned short  ITouch::available(){
    // Serial.printf("ITouch::available\n");
    if (checkch()==0) return 0; 
    return xy.length();
}
char ITouch::inch(){
    // Serial.printf("ITouch::inch\n");
    char c=checkch();
    if (c!=0) {
        xyAt++;
    }
    return c;   
}
void ITouch::ins(char *b, unsigned short nb){
    int length=0;
    char *buf=&b[1];
    if (checkch()==0) return;
    // Serial.printf("ITouch::ins\n");
    while(length<nb && length<xy.length() ) {
        buf[length++] = xy.charAt(xyAt++);
        if (xy.charAt(xyAt-1)=='\n'){
            if (xy.length()==xyAt) {
                xyAt=-1; 
            }
            buf[length]=0;
            b[0] = length;
            // Serial.printf("1ITouch::ins%d(%s)\n",buf[0],&buf[1]);
            return;
        } 
    }
    buf[length]=0;
    // Serial.printf("2ITouch::ins%d(%s)\n",buf[0],&buf[1]);
    b[0] = length;
    return;
}   
bool ITouch::getNextTouch(){
    // Serial.printf("ITouch::getNextTouch\n");
            int evt,x,y;
        if(gTouchQueue.pop(evt,x,y)){
            event = String(evt);
            // xy=String(x)+","+String(y)+"\n";
            xy=String(evt)+"\n"+String(x)+"\n"+String(y)+"\n";
            xyAt=0; eventAt=0;
            // Serial.printf("ITouch::getNextTouch is pop (%s) " , xy.c_str() );
            return true;
        }else {
            xyAt=-1; 
            return false;
        }
}
void   ITouch::flushTouch(){
        gTouchQueue.flush();
        xy=String();
        event=String();
        xyAt=-1; // enmpty xy
        eventAt=0;
}

ITouch iTouch;

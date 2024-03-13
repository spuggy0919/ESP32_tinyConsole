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
#include "ESP32inc.h"
#include "plotLabel.h"
#include <string.h>

plotLabel timer,date;
/* change cmd_example as your cmd name */


int cmd_time(int argc,char *argv[]){
    COLOR bk=0x404040FF,fg=0xFF0000FF;
    // for time HH:MM
    float atX=0.044,atY=0.7;
    int fSize=200;
    // for date  
    float atXD=0.046,atYD=0.2;
    int fSizeD=80;
    String hhmmString = timerCurrent().substring(0,5);
    String dateString = "(五)\n"+(timerDate().substring(5,11));
    if (argc>=1) { //graphics mode
        if (argc>=2) {
            fg =  strtoul(argv[1], NULL, 16);
        }
        if (argc>=3) {
            bk =  strtoul(argv[2], NULL, 16);
        }
        if (argc>=4) { 
            atX =  String(argv[3]).toFloat();
        }
        if (argc>=5) { 
            atY =  String(argv[4]).toFloat();
        }
        if (argc==6) { 
            fSize =  atoi(argv[5]);
        }
        
        timer.box(0.0,0.30,1.0,1.0); // 全畫面座標（0,0)-(1.0,1.0)
        timer.fontColor(fg,bk);
        timer.label(hhmmString);
        timer.fontSize(fSize);
        timer.at(atX,atY);
        timer.plot();
        date.box(0.0,0.0,1.0,0.30); // 全畫面座標（0,0)-(1.0,1.0)
        date.fontColor(fg,bk);
        date.label(dateString);
        date.at(atXD,atYD);
        date.fontSize(fSizeD);
        date.plot(); 
     }

    while(!wsSerial.escape()) {
        if (argc==2 && String(argv[1])=="0") {
            String td = timerCurrent()+" "+timerDate();
            wsTextPrintln(td+"\n");
        }else{
            if (hhmmString != (timerCurrent().substring(0,5))) {
                hhmmString = timerCurrent().substring(0,5);
                timer.label(hhmmString);
                timer.plot();
            }
            if (dateString != ( "(五)\n"+(timerDate().substring(5,11)))) {
                dateString =  "(五)\n"+(timerDate().substring(5,11));
                date.label(dateString);
                date.plot();
            }           
         }
        delay(1000);
    }
    return 0; // true for continue
}
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

plotLabel::plotLabel(){}
plotLabel::plotLabel(String label){
    labelStr = label;
}
plotLabel::~plotLabel(){}
void plotLabel::at(float xi, float yi){
    xorg=PERCENTW(xi);yorg=PERCENTH(yi);
}
void plotLabel::box(float xi, float yi, float wi, float hi){
    x0=PERCENTW(xi);y0=PERCENTH(yi);
    x1=x0+PERCENTW(wi);y1=y0+PERCENTH(hi);
}

void plotLabel::label(String l){
    labelStr = l;
}
void plotLabel::fontColor(COLOR fg,COLOR bk){
    fgColor = fg;
    bkColor = bk;
}
void plotLabel::fontSize(int size){
    fSize = size;
}
void plotLabel::plot(){
    if (bkColor&0xff) { // no alpha
        // drawPenColor((bkColor&0xFFFFFF)|0xff000000);
        // drawFRect(x0,y0,x1,y1);           
        drawPenColor(bkColor);
            // wsTextPrintf("bk=%0x\n",bkColor);
        drawFRect(x0,y0,x1,y1);                
      
    }
    if (fgColor&0xff) { // no alpha
        drawSetFontSize(fSize);
        // drawPenColor((fgColor&0xFFFFFF)|0x80000000);
        // drawText(labelStr,xorg, yorg,0.0);
        drawSetFontColor(fgColor);
            // wsTextPrintf("fg=%0x\n",fgColor);
        drawText(labelStr,xorg, yorg,0.0);
    }
}


#ifndef _PLOT_LABEL_H_
#define _PLOT_LABEL_H_
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
#include "audiovideo.h"

class plotLabel{
    public:
        plotLabel(); 
        plotLabel(String label);
        ~plotLabel();
        void at(float xi, float yi);
        void box(float xi, float yi, float wi, float hi);
        void label(String l);
        void fontColor(COLOR fg,COLOR bk);
        void fontSize(int size);
        void plot();
    private:
        int  x0=0,y0=0,x1=DISP_W,y1=DISP_H;
        int  xorg=0,yorg=0;
        String labelStr="";
        COLOR fgColor=0xFF0000FF;
        COLOR bkColor=0x000000FF;
        int  fSize=15;
};

#endif //_PLOT_LABEL_H_
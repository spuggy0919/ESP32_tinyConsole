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
#ifndef _PLOT_LINE_CHAR_H_
#define _PLOT_LINE_CHAR_H_
#include "audiovideo.h"
#include <list>

class LineChart{
    public:
        LineChart(); 
        LineChart(COLOR bkcolor); 
        ~LineChart();
        void xyLabel(String x,String y);
        void dataLable(String hTitle,String yTitle);
        void dataColor(COLOR hColor,COLOR tColor);
        void insertData(float h,float t);
        void resetData();
        void plot();
    private:
        void clearScreen(COLOR color);
        void plotAxisLabel();
        void plotData(const int setnumber);
        std::list<std::tuple<float, float>> List;
        COLOR bkColor=0xFFFFFFFF; // white
        String xAxis="TIME";
        String yAxis="(%,C)";
        COLOR  xColor=0xFFFFFFFF; //0xFF00FFFF; // 
        COLOR  yColor=0xFFFFFFFF; //0xFFFFFF00; // blue
        String hLabel="Humidity";
        String tLabel="Tempature";        
        COLOR  hColor=0xFF0000FF; //0xFF0000FF; // blue
        COLOR  tColor=0x0000FFFF; //0xFFFF0000; // blue

};

#endif //_PLOT_LINE_CHAR_H_
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
#include "plotLineChart.h"

LineChart::LineChart(){
        resetData();
        clearScreen(bkColor);
}
LineChart::LineChart(COLOR bcolor){
        resetData();
        bkColor=bcolor;
        clearScreen(bcolor);
}
LineChart::~LineChart(){
        resetData();
}
void LineChart::xyLabel(String x,String y){
        xAxis = x;
        yAxis = y;
        plotAxisLabel();
}
void  LineChart::dataLable(String hL,String tL){
            hLabel = hL;
            tLabel = tL;
            plotAxisLabel();
}
void  LineChart::dataColor(COLOR hc, COLOR tc){
            hColor = hc;
            tColor = tc;
            plotAxisLabel();
}
void  LineChart::insertData(float h,float t){
        List.push_back(std::make_tuple(h, t));

        return;
}
void LineChart::resetData(){
        List.clear();
        return;
}
void LineChart::plot(){
        clearScreen(bkColor);
        plotAxisLabel();
        plotData(0);
        plotData(1);
}
void LineChart::clearScreen(COLOR color){
        drawPenColor(color);
        drawFRect(0,0,DISP_W,DISP_H);
        return;
}
void LineChart::plotAxisLabel(){
        // plot Y axis and Label
        drawStrokeWidth(3);
        // axis
        int fontsize = 24;
        float factor=0.1;
        int x0=PERCENTW(factor);
        int x1=PERCENTW(1.0-factor);
        int y0=PERCENTH(factor);
        int y1=PERCENTH(1.0-factor);
        drawPenColor(xColor);
        drawVLine(x0,y0,y1);
        drawPenColor(yColor);
        drawHLine(y1,x0,x1);
        // axis label
        drawSetFontSize(fontsize);
        drawSetFontColor(xColor);
        float xfactor=0.05;
        x0=PERCENTW(0.05);
        y0=PERCENTH(0.1);
        drawText(yAxis,x0, y0,0.0);
        x1=PERCENTW(1-factor);
        y1=PERCENTH(1-xfactor);
        drawSetFontColor(yColor);
        drawText(xAxis,x1, y1,0.0);
        // plot h t label box 
        xfactor = 0.8;
        float yfactor = 0.2;
        x0=PERCENTW(xfactor);
        y0=PERCENTH(yfactor);
        y1=PERCENTH(yfactor+0.12);
        drawSetFontSize(fontsize);
        drawSetFontColor(hColor);
        drawText(hLabel,x0, y0,0.0);
        drawSetFontColor(tColor);
        drawText(tLabel,x0, y1,0.0);
        return;
}
void  LineChart::plotData(const int setnumber){
        if (List.size()<10) return;
        if (List.size()>60) List.pop_front();
        int x0,x1,y0,y1;
        float xfactor;
         // Iterate through the list and print each element
        xfactor = 0.9/List.size();
        float xorg=0.15;
        bool first = true;
        float number;
        x0=PERCENTW(xorg);
        COLOR pColor = (setnumber) ? tColor:hColor;
        drawPenColor(pColor);
        for (const auto& element : List) {
                // std::cout << "(" << element.first << ", " << element.second << ") ";
                x1 = PERCENTW(xorg);
                number = (setnumber==1) ? std::get<1>(element)/50.0:std::get<0>(element)/100.0;
                 wsMonitorPrintf("%f,(%d,%d)\n",number,x1,y1); // websocket print to monitor
                y1 = PERCENTH(1.0-number); 
                if (first) {
                    first = false;
                    y0=y1;
                    x0=x1;
                    xorg+=xfactor;
                    continue;
                }
                drawLine(x0,y0,x1,y1);
                x0 = x1;
                y0 = y1;
                xorg+=xfactor;
        }

}

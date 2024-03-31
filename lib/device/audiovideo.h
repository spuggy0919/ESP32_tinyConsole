#ifndef __DEVICE_H__ 
#define __DEVICE_H__ 
#include <Arduino.h>
#include "Base64.h"

int video(int argc,char * argv[]);

#define DEVICE_LOG   1
#define DEVICE_AUDIO 8
#define DEVICE_VIDEO 9
void _wsDevice(const char dev,const char *fmt,...);

#define audiof(c) _wsDevice(DEVICE_AUDIO,(c))
#define videof(c) _wsDevice(DEVICE_VIDEO,(c))
#define consoloLog(c) _wsDevice(DEVICE_LOG,(c))



// ClearScr,
// drawFilledRect,
// drawPixel,
// drawText,
// drawHLine,
// drawVLine,
// drawLine,
// drawArc,
// drawFillArc,
// drawCircle,
// drawFillCircle,
// drawStrokeWidth,
// drawSetPenColor,
#define DRAW_CLEAR  0
#define DRAW_FRECT  1
#define DRAW_PIXEL  2
#define DRAW_TEXT   3
#define DRAW_HLINE  4
#define DRAW_VLINE  5
#define DRAW_LINE   6
#define DRAW_ARC    7
#define DRAW_FARC   8
#define DRAW_CIRCLE 9
#define DRAW_FCIRCLE 10
#define DRAW_STROKEWIDTH 11
#define DRAW_PENCOLOR 12
#define DRAW_RECT 13
#define DRAW_CHAR 14
#define DRAW_GET_IMAGE 15
#define DRAW_PUT_IMAGE 16
#define DRAW_SET_FONTSIZE 17 
#define DRAW_SET_FONTCOLOR 18
#define DRAW_IMAGE_FILE 19 /*TBD*/

#define drawClearScr() { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d",DRAW_CLEAR); \
    videof(vbuf); \
    }
#define drawRect( x, y, width, height) { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d %d %d %d %d",DRAW_RECT,(x),(y),(width),(height)); \
    videof(vbuf); \
    }
#define drawFRect( x, y, width, height) { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d %d %d %d %d",DRAW_FRECT,(x),(y),(width),(height)); \
    videof(vbuf); \
    }
#define drawPixel( x, y) { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d %d %d",DRAW_PIXEL,(x),(y)); \
    videof(vbuf); \
    }
// function drawText(text, x, y, deg) {
int drawText(String text,int x, int y,float deg) ;
int drawChar(char c);

// #define drawText( text, x, y, deg) { \
//     char vbuf[80]; \
//     int encodedLength = Base64.encodedLength(text.length()); \
//     char encodedString[encodedLength+1]; \
//     Base64.encode(encodedString, (char *) text.c_str(), text.length());  \
//     sprintf(vbuf,"%1d %s %d %d %d",DRAW_TEXT,encodedString,x,y,deg); \
//     videof(vbuf); \
//     }
#define drawHLine(y, x1, x2) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d %d %d",DRAW_HLINE,(y),(x1),(x2)); \
    videof(vbuf); \
    }
#define drawVLine( x, y1, y2) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d %d %d %d",DRAW_VLINE,(x),(y1),(y2)); \
    videof(vbuf); \
    }
#define drawLine( x, y, x1, y1) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d %d %d %d",DRAW_LINE,(x),(y),(x1),(y1)); \
    videof(vbuf); \
    }            
// function drawArc( x, y, radius, from, to )
#define drawArc( x, y, r, frm, tod) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d %d %d %f %f",DRAW_ARC,(x),(y),(r),(frm),(tod)); \
    videof(vbuf); \
    }
#define drawFArc( x, y, r, frm, tod) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d %d %d %f %f",DRAW_FARC,(x),(y),(r),(frm),(tod)); \
    videof(vbuf); \
    }
#define drawCircle( x, y, r) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d %d %d",DRAW_CIRCLE,(x),(y),(r)); \
    videof(vbuf); \
    }
#define drawFCircle( x, y, r) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d %d %d",DRAW_FCIRCLE,(x),(y),(r)); \
    videof(vbuf); \
    }
#define drawStrokeWidth( w ) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d",DRAW_STROKEWIDTH,(w)); \
    videof(vbuf); \
    }
#define drawPenColor( c ) { \
    char vbuf[80]; \
    sprintf(vbuf,"%d %d ",DRAW_PENCOLOR,(c)); \
    videof(vbuf); \
    }
#define getImage( x, y, width, height) { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d %d %d %d %d",DRAW_GET_IMAGE,(x),(y),(width),(height)); \
    videof(vbuf); \
    }
#define putImage( x, y) { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d %d %d",DRAW_PUT_IMAGE,(x),(y)); \
    videof(vbuf); \
    }
    
#define drawSetFontSize( s) { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d %d",DRAW_SET_FONTSIZE,(s)); \
    videof(vbuf); \
    }
#define drawSetFontColor( c) { \
    char vbuf[80]; \
    sprintf(vbuf,"%1d %d",DRAW_SET_FONTCOLOR,(c)); \
    videof(vbuf); \
    }

typedef unsigned long  COLOR;
#define DISP_W 640.0
#define DISP_H 480.0
#define PERCENTW(xf) ((int)(DISP_W*(xf)))
#define PERCENTH(yf) ((int)(DISP_H*(yf)))
#endif // __VIDEO_H__ 
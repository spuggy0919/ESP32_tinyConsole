/*
 * This file is part of ESP32_TinyConsole.
 *
 *  Copyright (c) 2024 spuggy0919
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
#include "jswrap_tc.h"

// int drawText(String text,int x, int y,float deg) { 
//     char vbuf[80]; 
//     int encodedLength = Base64.encodedLength(text.length());
//     char encodedString[encodedLength+1];
//     Base64.encode(encodedString, (char *) text.c_str(), text.length());  
//     sprintf(vbuf,"%1d %s %d %d %f",DRAW_TEXT,encodedString,x,y,deg); 
//     videof(vbuf); 
//     return 0;
// }
JERRYXX_DECLARE_FUNCTION(graphics_drawtext){
  JERRYX_UNUSED(call_info_p);
  char      buf[64];
  uint32_t  len=64;
  int32_t   x,y;
  double    deg;
  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_number(&deg, JERRYX_ARG_NO_COERCE , JERRYX_ARG_REQUIRED)
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));

  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

  drawText(String(buf),(int)x,(int)y,(float)deg);

  return jerry_undefined();
} /*js_graphics_drawtext*/

// int drawChar(char c) { 
//     char vbuf[80]; 
//     sprintf(vbuf,"%1d %d",DRAW_CHAR,c); 
//     videof(vbuf); 
//     return 0;
// }
JERRYXX_DECLARE_FUNCTION(graphics_drawchar){
  JERRYX_UNUSED(call_info_p);
  char      buf[8];
  uint32_t  len=8;
  int32_t   x,y;
  double    deg;
  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

  drawChar( buf[0]);
  return jerry_undefined();
} /*js_graphics_drawchar*/


// #define drawClearScr() { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d",DRAW_CLEAR); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawclearscr){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4
  drawClearScr();
  return jerry_undefined();
} /*js_graphics_drawclearscr*/

// #define drawRect( x, y, width, height) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d %d %d %d %d",DRAW_RECT,(x),(y),(width),(height)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawrect){
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y;
  int32_t   w,h;
  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

   drawRect((int) x,(int)  y,(int)  w, (int) h);
  return jerry_undefined();
} /*js_graphics_drawrect*/

// #define drawFRect( x, y, width, height) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d %d %d %d %d",DRAW_RECT,(x),(y),(width),(height)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawfrect){
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y;
  int32_t   w,h;
  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

   drawFRect((int) x,(int)  y,(int)  w, (int) h);
  return jerry_undefined();
} /*js_graphics_drawfrect*/

// #define drawPixel( x, y) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d %d %d",DRAW_PIXEL,(x),(y)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawpixel){
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y;
  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

   drawPixel((int) x,(int)  y);
  return jerry_undefined();
} /*js_graphics_drawpixel*/

// #define drawHLine(y, x1, x2) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d %d %d",DRAW_HLINE,(y),(x1),(x2)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawhline){
  JERRYX_UNUSED(call_info_p);
  int32_t   y;
  int32_t   x1,x2;
  const jerryx_arg_t mapping[] =
      {
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&x1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&x2, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count"); 

   drawHLine((int)y, (int)x1, (int)x2);
  return jerry_undefined();
} /*js_graphics_drawhline*/

// #define drawVLine( x, y1, y2) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d %d %d %d",DRAW_VLINE,(x),(y1),(y2)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawvline){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   x; //2
  int32_t   y1,y2;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y2, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");  //4

   drawVLine((int)x, (int)y1, (int)y2); //5
  return jerry_undefined();
} /*js_graphics_drawvline*/ //6

// #define drawLine( x, y, x1, y1) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d %d %d %d",DRAW_LINE,(x),(y),(x1),(y1)); \
//     videof(vbuf); \
//     }            
JERRYXX_DECLARE_FUNCTION(graphics_drawline){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y; //2
  int32_t   x1,y1;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&x1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");  //4

   drawLine((int)x, (int)y, (int)x1, (int)y1); //5
  return jerry_undefined();
} /*js_graphics_drawline*/ //6

// function drawArc( x, y, radius, from, to )
// #define drawArc( x, y, r, frm, tod) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d %d %d %f %f",DRAW_ARC,(x),(y),(r),(frm),(tod)); \
//     videof(vbuf); \
//     }       
JERRYXX_DECLARE_FUNCTION(graphics_drawarc){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y,radius; //2
  double   frm,tod;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&radius, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_number(&frm, JERRYX_ARG_NO_COERCE , JERRYX_ARG_REQUIRED),
          jerryx_arg_number(&tod, JERRYX_ARG_NO_COERCE , JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");  //4

   drawArc((int)x, (int)y, (int)radius, (float)frm, (float)tod); //5
  return jerry_undefined();
} /*js_graphics_drawarc*/ //6

// function drawFArc( x, y, radius, from, to )
// #define drawFArc( x, y, r, frm, tod) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d %d %d %d %d",DRAW_FARC,(x),(y),(r),(frm),(tod)); \
//     videof(kbuf); \
//     } 
JERRYXX_DECLARE_FUNCTION(graphics_drawfarc){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y,radius; //2
  double   frm,tod;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&radius, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_number(&frm, JERRYX_ARG_NO_COERCE , JERRYX_ARG_REQUIRED),
          jerryx_arg_number(&tod, JERRYX_ARG_NO_COERCE , JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");  //4

   drawFArc((int)x, (int)y, (int)radius, (float)frm, (float)tod); //5
  return jerry_undefined();
} /*js_graphics_drawfarc*/ //6

// #define drawCircle( x, y, r) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d %d %d",DRAW_CIRCLE,(x),(y),(r)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawcircle){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y,radius; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&radius, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");  //4

   drawCircle((int)x, (int)y, (int)radius); //5
  return jerry_undefined();
} /*js_graphics_drawcircle*/ //6

// #define drawFCircle( x, y, r) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d %d %d",DRAW_FCIRCLE,(x),(y),(r)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawfcircle){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y,radius; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&radius, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");  //4

   drawFCircle((int)x, (int)y, (int)radius); //5
  return jerry_undefined();
} /*js_graphics_drawfcircle*/ //6

// #define drawStrokeWidth( w ) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d",DRAW_STROKEWIDTH,(w)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawstrokewidth){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   w; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

   drawStrokeWidth((int)w); //5
  return jerry_undefined();
} /*js_graphics_drawstrokewidth*/ //6

// #define drawPenColor( c ) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%d %d ",DRAW_PENCOLOR,(c)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawpencolor){ //1
  JERRYX_UNUSED(call_info_p);
  uint32_t   color; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_uint32(&color, JERRYX_ARG_CEIL, JERRYX_ARG_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

   drawPenColor((uint32_t)color); //5
  return jerry_undefined();
} /*js_graphics_drawpencolor*/ //6

// #define getImage( x, y, width, height) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d %d %d %d %d",DRAW_GET_IMAGE,(x),(y),(width),(height)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_getimage){ //1
  JERRYX_UNUSED(call_info_p);



  int32_t   x,y; //2
  int32_t   w,h;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");  //4

   getImage(x,y,w,h); //5
  return jerry_undefined();
} /*js_graphics_getimage*/ //6

// #define putImage( x, y, width, height) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d %d %d %d %d",DRAW_PUT_IMAGE,(x),(y),(width),(height)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_putimage){ //1
  JERRYX_UNUSED(call_info_p);
  int32_t   x,y; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");  //4

   putImage(x,y); //5
  return jerry_undefined();
} /*js_graphics_putimage*/ //6

// #define drawSetFontSize( s) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d %d",DRAW_SET_FONTSIZE,(s)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawsetfontsize){ //1
  JERRYX_UNUSED(call_info_p);
  uint32_t   fontsize; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_uint32(&fontsize, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

   drawSetFontSize((uint32_t)fontsize); //5
  return jerry_undefined();
} /*js_graphics_drawsetfontsize*/ //6

// #define drawSetFontColor( c) { \
//     char vbuf[80]; \
//     sprintf(vbuf,"%1d %d",DRAW_SET_FONTCOLOR,(c)); \
//     videof(vbuf); \
//     }
JERRYXX_DECLARE_FUNCTION(graphics_drawsetfontcolor){ //1
  JERRYX_UNUSED(call_info_p);
  uint32_t   color; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_uint32(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

   drawSetFontColor((uint32_t)color); //5
  return jerry_undefined();
} /*js_graphics_drawsetfontcolor*/ //6

JERRYXX_DECLARE_FUNCTION(graphics_classname){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");  //4

  return jerry_string_sz("Canvas");
} /*js_graphics_classname*/ //6

// class object wrapper
bool js_graphics_classobj_wraper(){
  bool ret = false;
          // Register initialization function
  jerryx_property_entry methods[] ={
    JERRYX_PROPERTY_FUNCTION ("name", js_graphics_classname),
    JERRYX_PROPERTY_FUNCTION ("drawClearScr", js_graphics_drawclearscr),
    JERRYX_PROPERTY_FUNCTION ("drawRect", js_graphics_drawrect),
    JERRYX_PROPERTY_FUNCTION ("drawFRect", js_graphics_drawfrect),
    JERRYX_PROPERTY_FUNCTION ("drawText", js_graphics_drawtext),
    JERRYX_PROPERTY_FUNCTION ("drawPixel", js_graphics_drawpixel),
    JERRYX_PROPERTY_FUNCTION ("drawHLine", js_graphics_drawhline),
    JERRYX_PROPERTY_FUNCTION ("drawVLine", js_graphics_drawvline),
    JERRYX_PROPERTY_FUNCTION ("drawLine", js_graphics_drawline),
    JERRYX_PROPERTY_FUNCTION ("drawArc", js_graphics_drawarc),
    JERRYX_PROPERTY_FUNCTION ("drawFArc", js_graphics_drawfarc),
    JERRYX_PROPERTY_FUNCTION ("drawCircle", js_graphics_drawcircle),
    JERRYX_PROPERTY_FUNCTION ("drawFCircle", js_graphics_drawfcircle),
    JERRYX_PROPERTY_FUNCTION ("drawStrokeWidth", js_graphics_drawstrokewidth),
    JERRYX_PROPERTY_FUNCTION ("drawPenColor", js_graphics_drawpencolor),
    JERRYX_PROPERTY_FUNCTION ("getImage", js_graphics_getimage),
    JERRYX_PROPERTY_FUNCTION ("putImage", js_graphics_putimage),

    JERRYX_PROPERTY_FUNCTION ("drawSetFontSize", js_graphics_drawsetfontsize),
    JERRYX_PROPERTY_FUNCTION ("drawSetFontColor", js_graphics_drawsetfontcolor),


    JERRYX_PROPERTY_LIST_END(),
  };


  jerry_value_t object = jerry_object();
  jerryx_register_result reg = jerryx_set_properties (object, methods);

  if (jerry_value_is_exception (reg.result))
  {
    wsTextPrintf ("Only registered %d properties\r\n", reg.registered);
    /* clean up not registered property values */
    jerryx_release_property_entry (methods, reg);
    jerry_value_free (reg.result);
    return false;
    /* clean up the error */
  }
  /* Set the native function as a property of the empty JS object */

  // wrap to dht 
  jerry_value_t globalObject = jerry_current_realm ();
  jerry_value_t prop_name = jerry_string_sz((const char*)"Canvas");
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}
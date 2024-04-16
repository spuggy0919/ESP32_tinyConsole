//Json:/js/sd1306.json
//File:function () { /* ecmascript */ }
//className:Adafruit_SSD1306
//classObj:sd1306
//constructorPrototype:Adafruit_SSD1306::Adafruit_SSD1306(uint8_t w, uint8_t h, int8_t rst_pin, uint32_t clkDuring , uint32_t clkAfter);
//prototypesArray:
//   bool begin(uint8_t switchvcc, uint8_t i2caddr, bool reset, bool periphBegin);
//   void display();
//   void clearDisplay();
//   void invertDisplay(bool i);
//   void dim(bool dim);
//   void drawPixel(int16_t x, int16_t y, uint16_t color);
//   void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
//   void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
//   void startscrollright(uint8_t start, uint8_t stop);
//   void startscrollleft(uint8_t start, uint8_t stop);
//   void startscrolldiagright(uint8_t start, uint8_t stop);
//   void startscrolldiagleft(uint8_t start, uint8_t stop);
//   void stopscroll();
//   void ssd1306_command(uint8_t c);
//   bool getPixel(int16_t x, int16_t y);

//   sd1306GFX 
//   void startWrite();
//   void writePixel(int16_t x, int16_t y, uint16_t color);
//   void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//   void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
//   void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
//   void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
//   void endWrite();
//   void setRotation(uint8_t r);
//   void invertDisplay(bool i);
//   void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//   void fillScreen(uint16_t color);
//   void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
//   void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//   void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
//   void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
//   void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
//   void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
//   void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
//   void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
//   void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
//   void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
//   void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, uint16_t color);
//   void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
//   void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h);
//   void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t* bitmap, uint8_t* mask, int16_t w, int16_t h);
//   void drawRGBBitmap(int16_t x, int16_t y, uint16_t* bitmap, int16_t w, int16_t h);
//   void drawRGBBitmap(int16_t x, int16_t y, uint16_t* bitmap, uint8_t* mask, int16_t w, int16_t h);
//   void drawChar(int16_t x, int16_t y,char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
//   void getTextBounds(char* str, int16_t x, int16_t y, int16_t* x1, int16_t* y1, uint16_t* w, uint16_t* h);
//   void setTextSize(uint8_t sx, uint8_t sy);
//   void setTextSize(uint8_t s);
//   void setFont(uint8_t* f);
//   void setCursor(int16_t x, int16_t y)
//   void setTextColor(uint16_t c)
//   void setTextColor(uint16_t c, uint16_t bg)
//   void setTextWrap(bool w);
//   void cp437(bool x);
//   void println(char*  str);
//   size_t write(uint8_t c);
//   int16_t width();
//   int16_t height();
//   uint8_t getRotation();
//   int16_t getCursorX();
//   int16_t getCursorY();
#include "jswrap_tc.h"
#ifdef _LANG_JERRYSCRIPT_ 
#ifdef _LIB_ADAFRUIT_SSD1306_
/* **HOW 1** simple select copy & paste **/
/* **HOW 2** or using process.argv[2] for output file, then dl file**/
/* **WARNNING** if tab position is wrong, means error found**/
/* **TODO** Include your class declaration here **/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
void sd1306_free_callback(void* native_p, jerry_object_native_info_t *info_p);
void native_image_free_callback(void* native_p, jerry_object_native_info_t *info_p){
     native_p = nullptr;
}
const static jerry_object_native_info_t native_image_info = {
    .free_cb = native_image_free_callback
};
const static jerry_object_native_info_t sd1306_info = {
    .free_cb = sd1306_free_callback
};
//bool begin(uint8_t switchvcc, uint8_t i2caddr, bool reset, bool periphBegin);
JS_METHOD(sd1306_begin)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t switchvcc = SSD1306_SWITCHCAPVCC;
    uint8_t i2caddr = 0;
    bool reset = true;
    bool periphBegin = true;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&switchvcc, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&i2caddr, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_boolean(&reset,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
        jerryx_arg_boolean(&periphBegin,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt < 2 , "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_boolean(sd1306->begin((uint8_t) switchvcc,(uint8_t) i2caddr,(bool) reset,(bool) periphBegin));
    }
    return jerry_undefined();
} /*js_sd1306_begin*/
//void display();
JS_METHOD(sd1306_display)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->display();
    }
    return jerry_undefined();
} /*js_sd1306_display*/
//void clearDisplay();
JS_METHOD(sd1306_clearDisplay)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->clearDisplay();
    }
    return jerry_undefined();
} /*js_sd1306_clearDisplay*/
//void invertDisplay(bool i);
JS_METHOD(sd1306_invertDisplay)
{
    JERRYX_UNUSED(call_info_p);
    bool i;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_boolean(&i,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->invertDisplay((bool) i);
    }
    return jerry_undefined();
} /*js_sd1306_invertDisplay*/
//void dim(bool dim);
JS_METHOD(sd1306_dim)
{
    JERRYX_UNUSED(call_info_p);
    bool dim;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_boolean(&dim,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->dim((bool) dim);
    }
    return jerry_undefined();
} /*js_sd1306_dim*/
//void drawPixel(int16_t x, int16_t y, uint16_t color);
JS_METHOD(sd1306_drawPixel)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawPixel( x, y, color);
    }
    return jerry_undefined();
} /*js_sd1306_drawPixel*/
//void startWrite();
JS_METHOD(sd1306_startWrite)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->startWrite();
    }
    return jerry_undefined();
} /*js_sd1306_startWrite*/
//void writePixel(int16_t x, int16_t y, uint16_t color);
JS_METHOD(sd1306_writePixel)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->writePixel((int16_t) x,(int16_t) y,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_writePixel*/
//void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
JS_METHOD(sd1306_writeFillRect)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->writeFillRect((int16_t) x,(int16_t) y,(int16_t) w,(int16_t) h,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_writeFillRect*/
//void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
JS_METHOD(sd1306_writeFastVLine)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    int16_t h;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->writeFastVLine((int16_t) x,(int16_t) y,(int16_t) h,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_writeFastVLine*/
//void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
JS_METHOD(sd1306_writeFastHLine)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    int16_t w;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->writeFastHLine((int16_t) x,(int16_t) y,(int16_t) w,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_writeFastHLine*/
//void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
JS_METHOD(sd1306_writeLine)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t x1;
    int16_t y1;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&x1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->writeLine((int16_t) x0,(int16_t) y0,(int16_t) x1,(int16_t) y1,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_writeLine*/
//void endWrite();
JS_METHOD(sd1306_endWrite)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->endWrite();
    }
    return jerry_undefined();
} /*js_sd1306_endWrite*/
//void setRotation(uint8_t r);
JS_METHOD(sd1306_setRotation)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t r;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&r, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->setRotation((uint8_t) r);
    }
    return jerry_undefined();
} /*js_sd1306_setRotation*/

//void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
JS_METHOD(sd1306_fillRect)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->fillRect((int16_t) x,(int16_t) y,(int16_t) w,(int16_t) h,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_fillRect*/
//void fillScreen(uint16_t color);
JS_METHOD(sd1306_fillScreen)
{
    JERRYX_UNUSED(call_info_p);
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->fillScreen((uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_fillScreen*/
//void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
JS_METHOD(sd1306_drawLine)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t x1;
    int16_t y1;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&x1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawLine((int16_t) x0,(int16_t) y0,(int16_t) x1,(int16_t) y1,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawLine*/
//void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
JS_METHOD(sd1306_drawRect)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawRect((int16_t) x,(int16_t) y,(int16_t) w,(int16_t) h,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawRect*/
//void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
JS_METHOD(sd1306_drawCircle)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t r;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&r, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawCircle((int16_t) x0,(int16_t) y0,(int16_t) r,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawCircle*/
//void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
JS_METHOD(sd1306_drawCircleHelper)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t r;
    uint8_t cornername;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&r, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&cornername, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawCircleHelper((int16_t) x0,(int16_t) y0,(int16_t) r,(uint8_t) cornername,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawCircleHelper*/
//void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
JS_METHOD(sd1306_fillCircle)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t r;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&r, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->fillCircle((int16_t) x0,(int16_t) y0,(int16_t) r,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_fillCircle*/
//void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
JS_METHOD(sd1306_fillCircleHelper)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t r;
    uint8_t cornername;
    int16_t delta;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&r, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&cornername, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&delta, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 6, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->fillCircleHelper((int16_t) x0,(int16_t) y0,(int16_t) r,(uint8_t) cornername,(int16_t) delta,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_fillCircleHelper*/
//void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
JS_METHOD(sd1306_drawTriangle)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&x1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&x2, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y2, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 7, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawTriangle((int16_t) x0,(int16_t) y0,(int16_t) x1,(int16_t) y1,(int16_t) x2,(int16_t) y2,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawTriangle*/
//void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
JS_METHOD(sd1306_fillTriangle)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&x1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y1, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&x2, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y2, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 7, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->fillTriangle((int16_t) x0,(int16_t) y0,(int16_t) x1,(int16_t) y1,(int16_t) x2,(int16_t) y2,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_fillTriangle*/
//void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
JS_METHOD(sd1306_drawRoundRect)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t w;
    int16_t h;
    int16_t radius;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&radius, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 6, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawRoundRect((int16_t) x0,(int16_t) y0,(int16_t) w,(int16_t) h,(int16_t) radius,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawRoundRect*/
//void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
JS_METHOD(sd1306_fillRoundRect)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x0;
    int16_t y0;
    int16_t w;
    int16_t h;
    int16_t radius;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y0, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&radius, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 6, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->fillRoundRect((int16_t) x0,(int16_t) y0,(int16_t) w,(int16_t) h,(int16_t) radius,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_fillRoundRect*/

//void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
JS_METHOD(sd1306_drawFastHLine)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    int16_t w;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawFastHLine((int16_t) x,(int16_t) y,(int16_t) w,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawFastHLine*/
//void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
JS_METHOD(sd1306_drawFastVLine)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    int16_t h;
    uint16_t color;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->drawFastVLine((int16_t) x,(int16_t) y,(int16_t) h,(uint16_t) color);
    }
    return jerry_undefined();
} /*js_sd1306_drawFastVLine*/

//void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
JS_METHOD(sd1306_drawBitmap)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x=1;
    int16_t y=2;
    uint8_t *bitmap;// **WARNNING** need to check BUFFERSIZE
    char    buf[32];// **WARNNING** need to check BUFFERSIZE
    int16_t w=3;
    int16_t h=4;
    uint16_t color=1;
    uint16_t bg=0;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_ignore(), // TODO process bitmap pointer later
        jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&bg, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    jerry_value_t rv = jerryx_arg_transform_args(args_p, 7, mapping, 7);
    if (jerry_value_is_exception(rv)){
        WSDEBUG_TPRINTF("[sd1306] drawBitmap mapping Error argc(%d) %x\n",args_cnt,rv);
        WSDEBUG_TPRINTF("[sd1306] x%d y%d w%d h%d color%d bg%d \n",x,y,w,h,color,bg);

        return rv;
    }

    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(!(args_cnt == 6||args_cnt == 7), "Wrong arguments count");
    /* process bitmap with ArrayBuffer*/
    if (jerry_value_is_arraybuffer(args_p[2])) {
        size_t size = jerry_arraybuffer_size (args_p[2]);
        WSDEBUG_TPRINTF("[sd1306] bitmap is arraybuf %x, argc(%d) length=%d\n",args_p[0],args_cnt,size);
        bitmap = jerry_arraybuffer_data (args_p[2]);

        void *native_pointer = NULL;
        native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            if (args_cnt==6) 
            sd1306->drawBitmap((int16_t) x,(int16_t) y,(uint8_t*) bitmap,(int16_t) w,(int16_t) h,(uint16_t) color);
            else
            sd1306->drawBitmap((int16_t) x,(int16_t) y,(uint8_t*) bitmap,(int16_t) w,(int16_t) h,(uint16_t) color,(uint16_t) bg);
        }
    }
    return jerry_undefined();
} /*js_sd1306_drawBitmap*/

//void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t* bitmap, [uint8_t* mask,] int16_t w, int16_t h);
JS_METHOD(sd1306_drawGrayscaleBitmap)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    uint8_t *bitmap;// **WARNNING** need to check BUFFERSIZE
    uint8_t *mask;// **WARNNING** need to check BUFFERSIZE
    int16_t w;
    int16_t h;

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);
    if (args_cnt==5){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_ignore(), // process bitmap pointer later
            jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");
        bitmap = jerry_arraybuffer_data (args_p[2]);

        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->drawGrayscaleBitmap((int16_t) x,(int16_t) y,(uint8_t*) bitmap,(int16_t) w,(int16_t) h);
        }
    }else{ // args_cnt=6 
        const jerryx_arg_t mapping[] ={
            jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_ignore(), // process bitmap pointer later
            jerryx_arg_ignore(), // process mask pointer later
            jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 6, "Wrong arguments count");
        bitmap = jerry_arraybuffer_data (args_p[2]);
        mask = jerry_arraybuffer_data (args_p[3]);

        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->drawGrayscaleBitmap((int16_t) x,(int16_t) y, bitmap, mask,(int16_t) w,(int16_t) h);
        }
    }
    return jerry_undefined();
} /*js_sd1306_drawGrayscaleBitmap*/

//void drawRGBBitmap(int16_t x, int16_t y, uint16_t* bitmap, [uint16_t* mask,] int16_t w, int16_t h);
JS_METHOD(sd1306_drawRGBBitmap)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    uint8_t* mask;
    uint8_t* bitmap;
    int16_t w;
    int16_t h;

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if (args_cnt==5){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_ignore(), // TODO process bitmap pointer later
            jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 5, "Wrong arguments count");
        bitmap = jerry_arraybuffer_data (args_p[2]);  // add array buffer 


        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->drawRGBBitmap((int16_t) x,(int16_t) y,(uint16_t*) bitmap,(int16_t) w,(int16_t) h);
        }
    }else{
        const jerryx_arg_t mapping[] ={
            jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_ignore(), // TODO process bitmap pointer later
        jerryx_arg_ignore(), // TODO process bitmap pointer later
            jerryx_arg_int16(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int16(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 6, "Wrong arguments count");
        bitmap = jerry_arraybuffer_data (args_p[2]);
        mask = jerry_arraybuffer_data (args_p[3]);

        void *native_pointer = NULL;
        native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->drawRGBBitmap((int16_t) x,(int16_t) y,(uint16_t*) bitmap, mask, (int16_t) w,(int16_t) h);
        }
    }
    return jerry_undefined();
} /*js_sd1306_drawRGBBitmap*/

//void startscrollright(uint8_t start, uint8_t stop);
JS_METHOD(sd1306_startscrollright)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t start;
    uint8_t stop;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&start, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&stop, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->startscrollright((uint8_t) start,(uint8_t) stop);
    }
    return jerry_undefined();
} /*js_sd1306_startscrollright*/
//void startscrollleft(uint8_t start, uint8_t stop);
JS_METHOD(sd1306_startscrollleft)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t start;
    uint8_t stop;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&start, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&stop, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->startscrollleft((uint8_t) start,(uint8_t) stop);
    }
    return jerry_undefined();
} /*js_sd1306_startscrollleft*/
//void startscrolldiagright(uint8_t start, uint8_t stop);
JS_METHOD(sd1306_startscrolldiagright)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t start;
    uint8_t stop;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&start, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&stop, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->startscrolldiagright((uint8_t) start,(uint8_t) stop);
    }
    return jerry_undefined();
} /*js_sd1306_startscrolldiagright*/
//void startscrolldiagleft(uint8_t start, uint8_t stop);
JS_METHOD(sd1306_startscrolldiagleft)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t start;
    uint8_t stop;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&start, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&stop, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->startscrolldiagleft((uint8_t) start,(uint8_t) stop);
    }
    return jerry_undefined();
} /*js_sd1306_startscrolldiagleft*/
//void stopscroll();
JS_METHOD(sd1306_stopscroll)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->stopscroll();
    }
    return jerry_undefined();
} /*js_sd1306_stopscroll*/
//void ssd1306_command(uint8_t c);
JS_METHOD(sd1306_ssd1306_command)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t c;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&c, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->ssd1306_command((uint8_t) c);
    }
    return jerry_undefined();
} /*js_sd1306_ssd1306_command*/
//bool getPixel(int16_t x, int16_t y);
JS_METHOD(sd1306_getPixel)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_boolean(sd1306->getPixel((int16_t) x,(int16_t) y));
    }
    return jerry_undefined();
} /*js_sd1306_getPixel*/
//void stopscroll();
JS_METHOD(sd1306_getBuffer)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        // sd1306->stopscroll(); Dummy call, no idea for return buffer
    }
    return jerry_undefined();
} /*js_sd1306_getBuffer*/
//void drawChar(int16_t x, int16_t y,char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
JS_METHOD(sd1306_drawChar)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;
    uint8_t c;
    uint16_t color;
    uint16_t bg;
    uint8_t size_x;
    uint8_t size_y;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&c, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&color, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&bg, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&size_x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&size_y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(!(args_cnt == 7 || args_cnt == 6), "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        if (args_cnt==6) 
        sd1306->drawChar((int16_t) x,(int16_t) y,(char) c,(uint16_t) color,(uint16_t) bg,(uint8_t) size_x);
        else
        sd1306->drawChar((int16_t) x,(int16_t) y,(char) c,(uint16_t) color,(uint16_t) bg,(uint8_t) size_x,(uint8_t) size_y);
    }
    return jerry_undefined();
} /*js_sd1306_drawChar*/
//void getTextBounds(char* str, int16_t x, int16_t y, int16_t* x1, int16_t* y1, uint16_t* w, uint16_t* h);
JS_METHOD(sd1306_getTextBounds)
{
    JERRYX_UNUSED(call_info_p);
    char str[64];// **WARNNING** need to check BUFFERSIZE
    int16_t x;
    int16_t y;
    int16_t* x1;
    int16_t* y1;
    uint16_t* w;
    uint16_t* h;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_string(str, sizeof(str), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_ignore(), //(&x1, JERRYX_ARG_REQUIRED),
        jerryx_arg_ignore(), //(&y1, JERRYX_ARG_REQUIRED),
        jerryx_arg_ignore(), //(&w, JERRYX_ARG_REQUIRED),
        jerryx_arg_ignore(), //(&h, JERRYX_ARG_REQUIRED),

    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 7, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->getTextBounds((char*) str,(int16_t) x,(int16_t) y,(int16_t*) x1,(int16_t*) y1,(uint16_t*) w,(uint16_t*) h);
        // TODO check call by refernce back to JS
    }
    return jerry_undefined();
} /*js_sd1306_getTextBounds*/
//void setTextSize(uint8_t sx, uint8_t sy);
JS_METHOD(sd1306_setTextSize)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t sx;
    uint8_t sy;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&sx, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&sy, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        if (args_cnt == 1) sd1306->setTextSize((uint8_t) sx);
        else sd1306->setTextSize((uint8_t) sx,(uint8_t) sy);
    }
    return jerry_undefined();
} /*js_sd1306_setTextSize*/


//void setFont(char *f);
JS_METHOD(sd1306_setFont)
{
    JERRYX_UNUSED(call_info_p);
    char f[128];// **WARNNING** need to check BUFFERSIZE

    const jerryx_arg_t mapping[] ={
        jerryx_arg_ignore(), //(&f, sizeof(f) 128JERRYX_ARG_OPTIONAL), // TODO 
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->setFont((const GFXfont *)f);
    }
    return jerry_undefined();
} /*js_sd1306_setFont*/
//void setCursor(int16_t x, int16_t y)
JS_METHOD(sd1306_setCursor)
{
    JERRYX_UNUSED(call_info_p);
    int16_t x;
    int16_t y;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int16(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int16(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->setCursor((int16_t) x,(int16_t) y);
    }
    return jerry_undefined();
} /*js_sd1306_setCursor*/
//void setTextColor(uint16_t c, uint16_t bg)
JS_METHOD(sd1306_setTextColor)
{
    JERRYX_UNUSED(call_info_p);
    uint16_t c;
    uint16_t bg;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint16(&c, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint16(&bg, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        if (args_cnt==1) sd1306->setTextColor((uint16_t) c);
        else sd1306->setTextColor((uint16_t) c,(uint16_t) bg);
    }
    return jerry_undefined();
} /*js_sd1306_setTextColor*/
//void setTextWrap(bool w);
JS_METHOD(sd1306_setTextWrap)
{
    JERRYX_UNUSED(call_info_p);
    bool w;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_boolean(&w,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->setTextWrap((bool) w);
    }
    return jerry_undefined();
} /*js_sd1306_setTextWrap*/
//void cp437(bool x);
JS_METHOD(sd1306_cp437)
{
    JERRYX_UNUSED(call_info_p);
    bool x = true;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_boolean(&x,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        sd1306->cp437((bool) x);
    }
    return jerry_undefined();
} /*js_sd1306_cp437*/
//size_t write(uint8_t c);
JS_METHOD(sd1306_write)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t c;
    char buf[2];
    if (jerry_value_is_string(args_p[0])){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        c=buf[0];

    }else{
        const jerryx_arg_t mapping[] ={
            jerryx_arg_uint8(&c, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }        
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_number(sd1306->write((uint8_t) c));
    }
    return jerry_undefined();
} /*js_sd1306_write*/
//int16_t width();
JS_METHOD(sd1306_width)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_number(sd1306->width());
    }
    return jerry_undefined();
} /*js_sd1306_width*/
//int16_t height();
JS_METHOD(sd1306_height)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_number(sd1306->height());
    }
    return jerry_undefined();
} /*js_sd1306_height*/
//uint8_t getRotation();
JS_METHOD(sd1306_getRotation)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_number(sd1306->getRotation());
    }
    return jerry_undefined();
} /*js_sd1306_getRotation*/
//int16_t getCursorX();
JS_METHOD(sd1306_getCursorX)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_number(sd1306->getCursorX());
    }
    return jerry_undefined();
} /*js_sd1306_getCursorX*/
//int16_t getCursorY();
JS_METHOD(sd1306_getCursorY)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);

    if(native_pointer) {
        Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
        return jerry_number(sd1306->getCursorY());
    }
    return jerry_undefined();
} /*js_sd1306_getCursorY*/
//void println(char*  str);
JS_METHOD(sd1306_print)
{
    JERRYX_UNUSED(call_info_p);
    char buf[64];// **WARNNING** need to check BUFFERSIZE
    double number;
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);
    if (jerry_value_is_number (args_p[0])){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_number(&number, JERRYX_ARG_NO_COERCE , JERRYX_ARG_REQUIRED)
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->print(buf);
        }
    }else{
        const jerryx_arg_t mapping[] ={
            jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");
        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->print(buf);
        }
    }
 
    return jerry_undefined();
} /*js_sd1306_print*/
//void println(char*  str);
JS_METHOD(sd1306_println)
{
    JERRYX_UNUSED(call_info_p);
    char buf[64];// **WARNNING** need to check BUFFERSIZE
    double number;
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &sd1306_info);
    if (jerry_value_is_number (args_p[0])){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_number(&number, JERRYX_ARG_NO_COERCE , JERRYX_ARG_REQUIRED)
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->println(buf);
        }
    }else{
        const jerryx_arg_t mapping[] ={
            jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");
        if(native_pointer) {
            Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_pointer;
            sd1306->println(buf);
        }
    }
 
    return jerry_undefined();
} /*js_sd1306_println*/
/* ~Adafruit_SSD1306 sd1306 destroy */
void sd1306_free_callback(void* native_p, jerry_object_native_info_t *info_p){
    WSDEBUG_TPRINTF("[Adafruit_SSD1306] delete %x\n",native_p);
    Adafruit_SSD1306* sd1306 = (Adafruit_SSD1306*)native_p;
    /** **TODO** free your own resource here **/
    delete(sd1306);
};
//Verify Methods List
/*js_sd1306_begin*/
/*js_sd1306_display*/
/*js_sd1306_clearDisplay*/
/*js_sd1306_invertDisplay*/
/*js_sd1306_dim*/
/*js_sd1306_drawPixel*/
/*js_sd1306_drawFastHLine*/
/*js_sd1306_drawFastVLine*/
/*js_sd1306_startscrollright*/
/*js_sd1306_startscrollleft*/
/*js_sd1306_startscrolldiagright*/
/*js_sd1306_startscrolldiagleft*/
/*js_sd1306_stopscroll*/
/*js_sd1306_ssd1306_command*/
/*js_sd1306_getPixel*/
/*js_sd1306_getBuffer*/
// GFX library inherit 
/*js_sd1306_setTextSize*/
/*js_sd1306_setFont*/
/*js_sd1306_setCursor*/
/*js_sd1306_setTextColor*/
/*js_sd1306_setTextWrap*/
/*js_sd1306_cp437*/
/*js_sd1306_println*/
/*js_sd1306_write*/
/*js_sd1306_width*/
/*js_sd1306_height*/
/*js_sd1306_getRotation*/
/*js_sd1306_getCursorX*/
/*js_sd1306_getCursorY*/
/*js_sd1306_println*/
//Adafruit_SSD1306::Adafruit_SSD1306(uint8_t w, uint8_t h, int8_t rst_pin, uint32_t clkDuring , uint32_t clkAfter);
JS_METHOD(sd1306_Adafruit_SSD1306)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t w;
    uint8_t h;
    int8_t rst_pin;
    uint32_t clkDuring;
    uint32_t clkAfter;
    Adafruit_SSD1306* sd1306;

    if (args_cnt == 1) {
        const jerryx_arg_t mapping[] ={
            jerryx_arg_int8(&rst_pin, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");
        sd1306 = new Adafruit_SSD1306((int8_t) rst_pin); //4
    }
    if (args_cnt >=2 ) {
        const jerryx_arg_t mapping[] ={
            jerryx_arg_uint8(&w, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_uint8(&h, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int8(&rst_pin, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
            jerryx_arg_uint32(&clkDuring, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
            jerryx_arg_uint32(&clkAfter, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt < 2, "Wrong arguments count");
        Adafruit_SSD1306* sd1306 = new Adafruit_SSD1306((uint8_t) w,(uint8_t) h); //4
    }
    WSDEBUG_TPRINTF("[Adafruit_SSD1306] new %x\n",sd1306);
    jerry_value_t js_sd1306obj = jerry_object();
    jerry_object_set_native_ptr(js_sd1306obj, &sd1306_info , sd1306 );
    // Register initialization function
    jerryx_property_entry props[] ={ //5 methods
        JERRYX_PROPERTY_STRING_SZ ("id", "Adafruit_SSD1306"),
        JERRYX_PROPERTY_FUNCTION ("begin", js_sd1306_begin),
        JERRYX_PROPERTY_FUNCTION ("display", js_sd1306_display),
        JERRYX_PROPERTY_FUNCTION ("clearDisplay", js_sd1306_clearDisplay),
        JERRYX_PROPERTY_FUNCTION ("invertDisplay", js_sd1306_invertDisplay),
        JERRYX_PROPERTY_FUNCTION ("dim", js_sd1306_dim),
        JERRYX_PROPERTY_FUNCTION ("drawPixel", js_sd1306_drawPixel),
        JERRYX_PROPERTY_FUNCTION ("drawFastHLine", js_sd1306_drawFastHLine),
        JERRYX_PROPERTY_FUNCTION ("drawFastVLine", js_sd1306_drawFastVLine),
        JERRYX_PROPERTY_FUNCTION ("startscrollright", js_sd1306_startscrollright),
        JERRYX_PROPERTY_FUNCTION ("startscrollleft", js_sd1306_startscrollleft),
        JERRYX_PROPERTY_FUNCTION ("startscrolldiagright", js_sd1306_startscrolldiagright),
        JERRYX_PROPERTY_FUNCTION ("startscrolldiagleft", js_sd1306_startscrolldiagleft),
        JERRYX_PROPERTY_FUNCTION ("stopscroll", js_sd1306_stopscroll),
        JERRYX_PROPERTY_FUNCTION ("ssd1306_command", js_sd1306_ssd1306_command),
        JERRYX_PROPERTY_FUNCTION ("getPixel", js_sd1306_getPixel),
        JERRYX_PROPERTY_FUNCTION ("getBuffer", js_sd1306_getBuffer),
        // GFX inherit 
        JERRYX_PROPERTY_FUNCTION ("startWrite", js_sd1306_startWrite),
        JERRYX_PROPERTY_FUNCTION ("writePixel", js_sd1306_writePixel),
        JERRYX_PROPERTY_FUNCTION ("writeFillRect", js_sd1306_writeFillRect),
        JERRYX_PROPERTY_FUNCTION ("writeFastVLine", js_sd1306_writeFastVLine),
        JERRYX_PROPERTY_FUNCTION ("writeFastHLine", js_sd1306_writeFastHLine),
        JERRYX_PROPERTY_FUNCTION ("writeLine", js_sd1306_writeLine),
        JERRYX_PROPERTY_FUNCTION ("endWrite", js_sd1306_endWrite),
        JERRYX_PROPERTY_FUNCTION ("setRotation", js_sd1306_setRotation),
        JERRYX_PROPERTY_FUNCTION ("invertDisplay", js_sd1306_invertDisplay),
        JERRYX_PROPERTY_FUNCTION ("fillRect", js_sd1306_fillRect),
        JERRYX_PROPERTY_FUNCTION ("fillScreen", js_sd1306_fillScreen),
        JERRYX_PROPERTY_FUNCTION ("drawLine", js_sd1306_drawLine),
        JERRYX_PROPERTY_FUNCTION ("drawRect", js_sd1306_drawRect),
        JERRYX_PROPERTY_FUNCTION ("drawCircle", js_sd1306_drawCircle),
        JERRYX_PROPERTY_FUNCTION ("drawCircleHelper", js_sd1306_drawCircleHelper),
        JERRYX_PROPERTY_FUNCTION ("fillCircle", js_sd1306_fillCircle),
        JERRYX_PROPERTY_FUNCTION ("fillCircleHelper", js_sd1306_fillCircleHelper),
        JERRYX_PROPERTY_FUNCTION ("drawTriangle", js_sd1306_drawTriangle),
        JERRYX_PROPERTY_FUNCTION ("fillTriangle", js_sd1306_fillTriangle),
        JERRYX_PROPERTY_FUNCTION ("drawRoundRect", js_sd1306_drawRoundRect),
        JERRYX_PROPERTY_FUNCTION ("fillRoundRect", js_sd1306_fillRoundRect),
        JERRYX_PROPERTY_FUNCTION ("drawBitmap", js_sd1306_drawBitmap),
        JERRYX_PROPERTY_FUNCTION ("drawGrayscaleBitmap", js_sd1306_drawGrayscaleBitmap),
        JERRYX_PROPERTY_FUNCTION ("drawRGBBitmap", js_sd1306_drawRGBBitmap),
        JERRYX_PROPERTY_FUNCTION ("drawChar", js_sd1306_drawChar),
        JERRYX_PROPERTY_FUNCTION ("getTextBounds", js_sd1306_getTextBounds),
        JERRYX_PROPERTY_FUNCTION ("setTextSize", js_sd1306_setTextSize),
        JERRYX_PROPERTY_FUNCTION ("setFont", js_sd1306_setFont),
        JERRYX_PROPERTY_FUNCTION ("setCursor", js_sd1306_setCursor),
        JERRYX_PROPERTY_FUNCTION ("setTextColor", js_sd1306_setTextColor),
        JERRYX_PROPERTY_FUNCTION ("setTextWrap", js_sd1306_setTextWrap),
        JERRYX_PROPERTY_FUNCTION ("cp437", js_sd1306_cp437),
        JERRYX_PROPERTY_FUNCTION ("println", js_sd1306_println),
        JERRYX_PROPERTY_FUNCTION ("write", js_sd1306_write),
        JERRYX_PROPERTY_FUNCTION ("width", js_sd1306_width),
        JERRYX_PROPERTY_FUNCTION ("height", js_sd1306_height),
        JERRYX_PROPERTY_FUNCTION ("getRotation", js_sd1306_getRotation),
        JERRYX_PROPERTY_FUNCTION ("getCursorX", js_sd1306_getCursorX),
        JERRYX_PROPERTY_FUNCTION ("getCursorY", js_sd1306_getCursorY),
        JERRYX_PROPERTY_LIST_END(),
    };
    jerryx_register_result reg = jerryx_set_properties (js_sd1306obj, props);
    if (jerry_value_is_exception (reg.result)){
        WSDEBUG_TPRINTF("Only registered %d properties\r\n", reg.registered);
        /* clean up not registered property values */
        jerryx_release_property_entry (props, reg);
        jerry_value_free (reg.result);
        return jerry_undefined();
        /* clean up the error */
    };
    return js_sd1306obj;
} /*js_sd1306_Adafruit_SSD1306*/
bool js_sd1306_classobj_wrapper(){  
//1 a)modified func name and b) define in jswrap_tc.h c) call by jswwrap_tc_module.cpp
    return jerryx_register_global ("Adafruit_SSD1306", js_sd1306_Adafruit_SSD1306);
};
#endif //_LIB_ADAFRUIT_SSD1306_
#endif // _LANG_JERRYSCRIPT_ Congraturation Done!

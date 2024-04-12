//Json:/js/LiquidCrystal.json
//File:function () { /* ecmascript */ }
//className:LiquidCrystal_I2C
//classObj:liquidlcd
//constructorPrototype:LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
//prototypesArray:
//   void begin(uint8_t cols, uint8_t rows, uint8_t charsize);
//   void clear();
//   void home();
//   void noDisplay();
//   void display();
//   void noBlink();
//   void blink();
//   void noCursor();
//   void cursor();
//   void scrollDisplayLeft();
//   void scrollDisplayRight();
//   void printLeft();
//   void printRight();
//   void leftToRight();
//   void rightToLeft();
//   void shiftIncrement();
//   void shiftDecrement();
//   void noBacklight();
//   void backlight();
//   void autoscroll();
//   void noAutoscroll();
//   void createChar(uint8_t location, uint8_t* charmap);
//   void setCursor(uint8_t x, uint8_t y);
//   size_t write(uint8_t c);
//   void command(uint8_t cmd);
//   void init();
//   void oledinit();
//   void blink_on();
//   void blink_off();
//   void cursor_on();
//   void cursor_off();
//   void setBacklight(uint8_t new_val);
//   void load_custom_character(uint8_t char_num, uint8_t* rows);
//   void printstr(char* buf);
#include "jswrap_tc.h"
#ifdef _LANG_JERRYSCRIPT_ 
#ifdef _LIB_LIQUIDCRYSTAL_I2C_
/* **HOW 1** simple select copy & paste **/
/* **HOW 2** or using process.argv[2] for output file, then dl file**/
/* **WARNNING** if tab position is wrong, means error found**/
/* **TODO** Include your class declaration here **/
#include "LiquidCrystal_I2C.h"
void liquidlcd_free_callback(void* native_p, jerry_object_native_info_t *info_p);
const static jerry_object_native_info_t liquidlcd_info = {
    .free_cb = liquidlcd_free_callback
};
//void begin(uint8_t cols, uint8_t rows, uint8_t charsize);
JS_METHOD(liquidlcd_begin)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t cols;
    uint8_t rows;
    uint8_t charsize= LCD_5x8DOTS;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&cols, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&rows, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&charsize, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(!((args_cnt == 2)||(args_cnt == 3)), "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->begin((uint8_t) cols,(uint8_t) rows,(uint8_t) charsize);
    }
    return jerry_undefined();
} /*js_liquidlcd_begin*/
//void clear();
JS_METHOD(liquidlcd_clear)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->clear();
    }
    return jerry_undefined();
} /*js_liquidlcd_clear*/
//void home();
JS_METHOD(liquidlcd_home)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->home();
    }
    return jerry_undefined();
} /*js_liquidlcd_home*/
//void noDisplay();
JS_METHOD(liquidlcd_noDisplay)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->noDisplay();
    }
    return jerry_undefined();
} /*js_liquidlcd_noDisplay*/
//void display();
JS_METHOD(liquidlcd_display)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->display();
    }
    return jerry_undefined();
} /*js_liquidlcd_display*/
//void noBlink();
JS_METHOD(liquidlcd_noBlink)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->noBlink();
    }
    return jerry_undefined();
} /*js_liquidlcd_noBlink*/
//void blink();
JS_METHOD(liquidlcd_blink)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->blink();
    }
    return jerry_undefined();
} /*js_liquidlcd_blink*/
//void noCursor();
JS_METHOD(liquidlcd_noCursor)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->noCursor();
    }
    return jerry_undefined();
} /*js_liquidlcd_noCursor*/
//void cursor();
JS_METHOD(liquidlcd_cursor)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->cursor();
    }
    return jerry_undefined();
} /*js_liquidlcd_cursor*/
//void scrollDisplayLeft();
JS_METHOD(liquidlcd_scrollDisplayLeft)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->scrollDisplayLeft();
    }
    return jerry_undefined();
} /*js_liquidlcd_scrollDisplayLeft*/
//void scrollDisplayRight();
JS_METHOD(liquidlcd_scrollDisplayRight)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->scrollDisplayRight();
    }
    return jerry_undefined();
} /*js_liquidlcd_scrollDisplayRight*/
//void printLeft();
// JS_METHOD(liquidlcd_printLeft)
// {
//     JERRYX_UNUSED(call_info_p);
//     JERRYX_UNUSED(args_p);
//     JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

//     void *native_pointer = NULL;
//     native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

//     if(native_pointer) {
//         LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
//         liquidlcd->printLeft();
//     }
//     return jerry_undefined();
// } /*js_liquidlcd_printLeft*/
// //void printRight();
// JS_METHOD(liquidlcd_printRight)
// {
//     JERRYX_UNUSED(call_info_p);
//     JERRYX_UNUSED(args_p);
//     JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

//     void *native_pointer = NULL;
//     native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

//     if(native_pointer) {
//         LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
//         liquidlcd->printRight();
//     }
//     return jerry_undefined();
// } /*js_liquidlcd_printRight*/
//void leftToRight();
JS_METHOD(liquidlcd_leftToRight)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->leftToRight();
    }
    return jerry_undefined();
} /*js_liquidlcd_leftToRight*/
//void rightToLeft();
JS_METHOD(liquidlcd_rightToLeft)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->rightToLeft();
    }
    return jerry_undefined();
} /*js_liquidlcd_rightToLeft*/
//void shiftIncrement();
// JS_METHOD(liquidlcd_shiftIncrement)
// {
//     JERRYX_UNUSED(call_info_p);
//     JERRYX_UNUSED(args_p);
//     JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

//     void *native_pointer = NULL;
//     native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

//     if(native_pointer) {
//         LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
//         liquidlcd->shiftIncrement();
//     }
//     return jerry_undefined();
// } /*js_liquidlcd_shiftIncrement*/
// //void shiftDecrement();
// JS_METHOD(liquidlcd_shiftDecrement)
// {
//     JERRYX_UNUSED(call_info_p);
//     JERRYX_UNUSED(args_p);
//     JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

//     void *native_pointer = NULL;
//     native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

//     if(native_pointer) {
//         LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
//         liquidlcd->shiftDecrement();
//     }
//     return jerry_undefined();
// } /*js_liquidlcd_shiftDecrement*/
//void noBacklight();
JS_METHOD(liquidlcd_noBacklight)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->noBacklight();
    }
    return jerry_undefined();
} /*js_liquidlcd_noBacklight*/
//void backlight();
JS_METHOD(liquidlcd_backlight)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->backlight();
    }
    return jerry_undefined();
} /*js_liquidlcd_backlight*/
//void autoscroll();
JS_METHOD(liquidlcd_autoscroll)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->autoscroll();
    }
    return jerry_undefined();
} /*js_liquidlcd_autoscroll*/
//void noAutoscroll();
JS_METHOD(liquidlcd_noAutoscroll)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->noAutoscroll();
    }
    return jerry_undefined();
} /*js_liquidlcd_noAutoscroll*/
//void createChar(uint8_t location, uint8_t* charmap);
JS_METHOD(liquidlcd_createChar)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t location;
    char charmap[32];

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&location, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_string(charmap, sizeof(charmap), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->createChar((uint8_t) location,(uint8_t*) charmap);
    }
    return jerry_undefined();
} /*js_liquidlcd_createChar*/
//void setCursor(uint8_t x, uint8_t y);
JS_METHOD(liquidlcd_setCursor)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t x;
    uint8_t y;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&x, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&y, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->setCursor((uint8_t) x,(uint8_t) y);
    }
    return jerry_undefined();
} /*js_liquidlcd_setCursor*/
//size_t write(uint8_t c);
JS_METHOD(liquidlcd_write)
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
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        return jerry_number(liquidlcd->write((uint8_t) c));
    }
    return jerry_undefined();
} /*js_liquidlcd_write*/
//void command(uint8_t cmd);
JS_METHOD(liquidlcd_command)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t cmd;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&cmd, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->command((uint8_t) cmd);
    }
    return jerry_undefined();
} /*js_liquidlcd_command*/
//void init();
JS_METHOD(liquidlcd_init)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->init();
    }
    return jerry_undefined();
} /*js_liquidlcd_init*/
//void oledinit();
JS_METHOD(liquidlcd_oledinit)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->oled_init();
    }
    return jerry_undefined();
} /*js_liquidlcd_oledinit*/
//void blink_on();
JS_METHOD(liquidlcd_blink_on)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->blink_on();
    }
    return jerry_undefined();
} /*js_liquidlcd_blink_on*/
//void blink_off();
JS_METHOD(liquidlcd_blink_off)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->blink_off();
    }
    return jerry_undefined();
} /*js_liquidlcd_blink_off*/
//void cursor_on();
JS_METHOD(liquidlcd_cursor_on)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->cursor_on();
    }
    return jerry_undefined();
} /*js_liquidlcd_cursor_on*/
//void cursor_off();
JS_METHOD(liquidlcd_cursor_off)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->cursor_off();
    }
    return jerry_undefined();
} /*js_liquidlcd_cursor_off*/
//void setBacklight(uint8_t new_val);
JS_METHOD(liquidlcd_setBacklight)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t new_val;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&new_val, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->setBacklight((uint8_t) new_val);
    }
    return jerry_undefined();
} /*js_liquidlcd_setBacklight*/
//void setContrast(uint8_t new_val);
JS_METHOD(liquidlcd_setContrast)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t new_val;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&new_val, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->setContrast((uint8_t) new_val);
    }
    return jerry_undefined();
} /*js_liquidlcd_setContrast*/
//void load_custom_character(uint8_t char_num, uint8_t* rows);
JS_METHOD(liquidlcd_load_custom_character)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t char_num;
    char rows[32];

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&char_num, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_string(rows, sizeof(rows), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->load_custom_character((uint8_t) char_num,(uint8_t*) rows);
    }
    return jerry_undefined();
} /*js_liquidlcd_load_custom_character*/
//void printstr(char* buf);
JS_METHOD(liquidlcd_printstr)
{
    JERRYX_UNUSED(call_info_p);
    char buf[32];

    const jerryx_arg_t mapping[] ={
        jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &liquidlcd_info);

    if(native_pointer) {
        LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_pointer;
        liquidlcd->print((char*) buf);
    }
    return jerry_undefined();
} /*js_liquidlcd_printstr*/
/* ~LiquidCrystal_I2C liquidlcd destroy */
void liquidlcd_free_callback(void* native_p, jerry_object_native_info_t *info_p){
    WSDEBUG_TPRINTF("[LiquidCrystal_I2C] delete %x\n",native_p);
    LiquidCrystal_I2C* liquidlcd = (LiquidCrystal_I2C*)native_p;
    /** **TODO** free your own resource here **/
    delete(liquidlcd);
};
//Verify Methods List
/*js_liquidlcd_begin*/
/*js_liquidlcd_clear*/
/*js_liquidlcd_home*/
/*js_liquidlcd_noDisplay*/
/*js_liquidlcd_display*/
/*js_liquidlcd_noBlink*/
/*js_liquidlcd_blink*/
/*js_liquidlcd_noCursor*/
/*js_liquidlcd_cursor*/
/*js_liquidlcd_scrollDisplayLeft*/
/*js_liquidlcd_scrollDisplayRight*/
/*js_liquidlcd_printLeft*/
/*js_liquidlcd_printRight*/
/*js_liquidlcd_leftToRight*/
/*js_liquidlcd_rightToLeft*/
/*js_liquidlcd_shiftIncrement*/
/*js_liquidlcd_shiftDecrement*/
/*js_liquidlcd_noBacklight*/
/*js_liquidlcd_backlight*/
/*js_liquidlcd_autoscroll*/
/*js_liquidlcd_noAutoscroll*/
/*js_liquidlcd_createChar*/
/*js_liquidlcd_setCursor*/
/*js_liquidlcd_write*/
/*js_liquidlcd_command*/
/*js_liquidlcd_init*/
/*js_liquidlcd_oledinit*/
/*js_liquidlcd_blink_on*/
/*js_liquidlcd_blink_off*/
/*js_liquidlcd_cursor_on*/
/*js_liquidlcd_cursor_off*/
/*js_liquidlcd_setBacklight*/
/*js_liquidlcd_setContrast*/
/*js_liquidlcd_load_custom_character*/
/*js_liquidlcd_printstr*/
//LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
JS_METHOD(liquidlcd_LiquidCrystal_I2C)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t lcd_Addr;
    uint8_t lcd_cols;
    uint8_t lcd_rows;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&lcd_Addr, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&lcd_cols, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint8(&lcd_rows, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");
    LiquidCrystal_I2C* liquidlcd = new LiquidCrystal_I2C((uint8_t) lcd_Addr,(uint8_t) lcd_cols,(uint8_t) lcd_rows); //4
    WSDEBUG_TPRINTF("[LiquidCrystal_I2C] new %x\n",liquidlcd);
    jerry_value_t js_liquidlcdobj = jerry_object();
    jerry_object_set_native_ptr(js_liquidlcdobj, &liquidlcd_info , liquidlcd );
    // Register initialization function
    jerryx_property_entry props[] ={ //5 methods
        JERRYX_PROPERTY_STRING_SZ ("id", "LiquidCrystal_I2C"),
        JERRYX_PROPERTY_FUNCTION ("begin", js_liquidlcd_begin),
        JERRYX_PROPERTY_FUNCTION ("clear", js_liquidlcd_clear),
        JERRYX_PROPERTY_FUNCTION ("home", js_liquidlcd_home),
        JERRYX_PROPERTY_FUNCTION ("noDisplay", js_liquidlcd_noDisplay),
        JERRYX_PROPERTY_FUNCTION ("display", js_liquidlcd_display),
        JERRYX_PROPERTY_FUNCTION ("noBlink", js_liquidlcd_noBlink),
        JERRYX_PROPERTY_FUNCTION ("blink", js_liquidlcd_blink),
        JERRYX_PROPERTY_FUNCTION ("noCursor", js_liquidlcd_noCursor),
        JERRYX_PROPERTY_FUNCTION ("cursor", js_liquidlcd_cursor),
        JERRYX_PROPERTY_FUNCTION ("scrollDisplayLeft", js_liquidlcd_scrollDisplayLeft),
        JERRYX_PROPERTY_FUNCTION ("scrollDisplayRight", js_liquidlcd_scrollDisplayRight),
        // JERRYX_PROPERTY_FUNCTION ("printLeft", js_liquidlcd_printLeft),
        // JERRYX_PROPERTY_FUNCTION ("printRight", js_liquidlcd_printRight),
        JERRYX_PROPERTY_FUNCTION ("leftToRight", js_liquidlcd_leftToRight),
        JERRYX_PROPERTY_FUNCTION ("rightToLeft", js_liquidlcd_rightToLeft),
        // JERRYX_PROPERTY_FUNCTION ("shiftIncrement", js_liquidlcd_shiftIncrement),
        // JERRYX_PROPERTY_FUNCTION ("shiftDecrement", js_liquidlcd_shiftDecrement),
        JERRYX_PROPERTY_FUNCTION ("noBacklight", js_liquidlcd_noBacklight),
        JERRYX_PROPERTY_FUNCTION ("backlight", js_liquidlcd_backlight),
        JERRYX_PROPERTY_FUNCTION ("autoscroll", js_liquidlcd_autoscroll),
        JERRYX_PROPERTY_FUNCTION ("noAutoscroll", js_liquidlcd_noAutoscroll),
        JERRYX_PROPERTY_FUNCTION ("createChar", js_liquidlcd_createChar),
        JERRYX_PROPERTY_FUNCTION ("setCursor", js_liquidlcd_setCursor),
        JERRYX_PROPERTY_FUNCTION ("write", js_liquidlcd_write),
        JERRYX_PROPERTY_FUNCTION ("command", js_liquidlcd_command),
        JERRYX_PROPERTY_FUNCTION ("init", js_liquidlcd_init),
        JERRYX_PROPERTY_FUNCTION ("oledinit", js_liquidlcd_oledinit),
        JERRYX_PROPERTY_FUNCTION ("blink_on", js_liquidlcd_blink_on),
        JERRYX_PROPERTY_FUNCTION ("blink_off", js_liquidlcd_blink_off),
        JERRYX_PROPERTY_FUNCTION ("cursor_on", js_liquidlcd_cursor_on),
        JERRYX_PROPERTY_FUNCTION ("cursor_off", js_liquidlcd_cursor_off),
        JERRYX_PROPERTY_FUNCTION ("setBacklight", js_liquidlcd_setBacklight),
        JERRYX_PROPERTY_FUNCTION ("setContrast", js_liquidlcd_setContrast),
        JERRYX_PROPERTY_FUNCTION ("load_custom_character", js_liquidlcd_load_custom_character),
        JERRYX_PROPERTY_FUNCTION ("print", js_liquidlcd_printstr),
        JERRYX_PROPERTY_LIST_END(),
    };
    jerryx_register_result reg = jerryx_set_properties (js_liquidlcdobj, props);
    if (jerry_value_is_exception (reg.result)){
        WSDEBUG_TPRINTF("Only registered %d properties\r\n", reg.registered);
        /* clean up not registered property values */
        jerryx_release_property_entry (props, reg);
        jerry_value_free (reg.result);
        return jerry_undefined();
        /* clean up the error */
    };
    return js_liquidlcdobj;
} /*js_liquidlcd_LiquidCrystal_I2C*/
bool js_liquidlcd_classobj_wrapper(){  
//1 a)modified func name and b) define in .h c) call by jswwrap_tc
    return jerryx_register_global ("LiquidCrystal_I2C", js_liquidlcd_LiquidCrystal_I2C);
};
#endif //_LIB_LIQUIDCRYSTAL_I2C_
#endif // _LANG_JERRYSCRIPT_ Congraturation Done!

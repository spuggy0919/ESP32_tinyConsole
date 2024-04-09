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

#ifdef _LIB_WIRE_

/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified December 2014 by Ivan Grokhotkov (ivan@esp8266.com) - esp8266 support
  Modified April 2015 by Hrsto Gochkov (ficeto@ficeto.com) - alternative esp8266 support
  Modified November 2017 by Chuck Todd <stickbreaker on GitHub> to use ISR and increase stability.
  Modified Nov 2021 by Hristo Gochkov <Me-No-Dev> to support ESP-IDF API
*/

void wire_free_callback(void* native_p, jerry_object_native_info_t *info_p);

const static jerry_object_native_info_t wire_info =
{
    .free_cb = wire_free_callback
};
// bool setPins(int sda, int scl);
JS_METHOD(setPins)
{
    JERRYX_UNUSED(call_info_p);
    int32_t sda;
    int32_t scl;
    const jerryx_arg_t mapping[] ={
        jerryx_arg_int32(&sda, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int32(&scl, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);
    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_boolean(wire->setPins(sda,scl)); //2
    }
    return jerry_undefined();
} /*js_setPins*/

    // bool begin(int sda, int scl, uint32_t frequency=0); // returns true, if successful init of i2c bus
    // bool begin(uint8_t slaveAddr, int sda, int scl, uint32_t frequency);
    // Explicit Overload for Arduino MainStream API compatibility
    // inline bool begin()
    // {
    //     return begin(-1, -1, static_cast<uint32_t>(0));
    // }
    // inline bool begin(uint8_t addr)
    // {
    //     return begin(addr, -1, -1, 0);
    // }
    // inline bool begin(int addr)
    // {
    //     return begin(static_cast<uint8_t>(addr), -1, -1, 0);
    // }
JS_METHOD(begin)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t slaveAddr;
    int32_t sda;
    int32_t scl;
    uint32_t frequency;
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

    if (args_cnt == 3){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_int32(&sda, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int32(&scl, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_uint32(&frequency, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            return jerry_boolean(wire->begin((int)sda,(int)scl,frequency)); //2
        }
    }else  if (args_cnt == 4){
        const jerryx_arg_t mapping[] ={ // slave mode
            jerryx_arg_uint32(&slaveAddr, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int32(&sda, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_int32(&scl, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_uint32(&frequency, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            // 
            return jerry_boolean(wire->begin((uint8_t)slaveAddr,(int)sda,(int)scl,frequency)); //2
        }
    }else  if (args_cnt == 1){
        const jerryx_arg_t mapping[] ={ // slave mode
            jerryx_arg_uint32(&slaveAddr, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            // 
            return jerry_boolean(wire->begin((uint8_t)slaveAddr)); //2
        }
    }else  if (args_cnt == 0){
    
        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            // 
            return jerry_boolean(wire->begin()); //2
        }
    }
    return jerry_undefined();
} /*js_begin*/
//size_t setBufferSize(size_t bSize);
JS_METHOD(setBufferSize)
{
    JERRYX_UNUSED(call_info_p);
    int32_t bSize;
    const jerryx_arg_t mapping[] ={
        jerryx_arg_int32(&bSize, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
            return rv;
        }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_number(wire->setBufferSize(bSize)); //2
    }
    return jerry_undefined();
} /*js_setBufferSize*/

//void setTimeOut(uint16_t timeOutMillis); // default timeout of i2c transactions is 50ms
JS_METHOD(setTimeOut)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t timeOutMillis;
    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&timeOutMillis, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
            return rv;
        }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        wire->setTimeOut((uint16_t)timeOutMillis);
    }
    return jerry_undefined();
} /*js_setTimeOut*/
//uint16_t getTimeOut();
JS_METHOD(getTimeOut)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_number(wire->getTimeOut());
    }
    return jerry_undefined();
} /*js_getTimeOut*/

//bool setClock(uint32_t clock);
JS_METHOD(setClock)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t clock;
    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&clock, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
            return rv;
        }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");


    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_boolean(wire->setClock(clock));
    }
    return jerry_undefined();
} /*js_setClock*/
//uint32_t getClock();
JS_METHOD(getClock)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_number(wire->getClock());
    }
    return jerry_undefined();
} /*js_getClock*/
//void beginTransmission(uint16_t address);
//void beginTransmission(uint8_t address);
// void beginTransmission(int address);
JS_METHOD(beginTransmission)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t address;
    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&address, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
            return rv;
        }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");


    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        wire->beginTransmission((uint8_t) address);
    }
    return jerry_undefined();
} /*js_beginTransmission*/
//uint8_t endTransmission(bool sendStop);
//uint8_t endTransmission();
JS_METHOD(endTransmission)
{
    JERRYX_UNUSED(call_info_p);
    bool sendStop;
    const jerryx_arg_t mapping[] ={
        jerryx_arg_boolean(&sendStop,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
        };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
            return rv;
        }


    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        if (args_cnt==1)
            return jerry_number(wire->endTransmission(sendStop));
        else
            return jerry_number(wire->endTransmission());
    }
    return jerry_undefined();
} /*js_endTransmission*/


// size_t requestFrom(uint16_t address, size_t size, bool sendStop);
// uint8_t requestFrom(uint16_t address, uint8_t size, bool sendStop);
// uint8_t requestFrom(uint16_t address, uint8_t size, uint8_t sendStop);
//size_t requestFrom(uint16_t address, size_t size, bool sendStop);
    // size_t requestFrom(uint8_t address, size_t len, bool stopBit);
    // uint8_t requestFrom(uint16_t address, uint8_t size);
    // uint8_t requestFrom(uint8_t address, uint8_t size, uint8_t sendStop);
    // uint8_t requestFrom(uint8_t address, uint8_t size);
    // uint8_t requestFrom(int address, int size, int sendStop);
    // uint8_t requestFrom(int address, int size);
JS_METHOD(requestFrom)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t address;
    uint32_t size;
    bool sendStop;

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

    if (args_cnt == 3) {
        const jerryx_arg_t mapping[] ={
            jerryx_arg_uint32(&address, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_uint32(&size, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_boolean(&sendStop,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
                return rv;
            }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");
        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            return jerry_number(wire->requestFrom((uint16_t) address,(size_t) size,sendStop));
        }
    }else if (args_cnt == 2){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_uint32(&address, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_uint32(&size, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
                return rv;
            }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");
        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            return jerry_number(wire->requestFrom((uint16_t) address,(size_t) size));
        }
    }
    return jerry_undefined();
} /*js_requestFrom*/


    // size_t write(uint8_t);
    // size_t write(const uint8_t *, size_t);
        // inline size_t write(const char * s)
    // {
    //     return write((uint8_t*) s, strlen(s));
    // }
    // inline size_t write(unsigned long n)
    // {
    //     return write((uint8_t)n);
    // }
    // inline size_t write(long n)
    // {
    //     return write((uint8_t)n);
    // }
    // inline size_t write(unsigned int n)
    // {
    //     return write((uint8_t)n);
    // }
    // inline size_t write(int n)
    // {
    //     return write((uint8_t)n);
    // }
//size_t write(uint8_t* buf, size_t len);
JS_METHOD(write)
{
    JERRYX_UNUSED(call_info_p);
    char buf[32];
    uint32_t  bytedata;
    uint32_t len;
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

    if (args_cnt==2) {
        const jerryx_arg_t mapping[] ={
            jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            jerryx_arg_uint32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
                return rv;
            }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");


        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            return jerry_number(wire->write((uint8_t *)buf,(size_t) len));
        }
    }else if(args_cnt==1) {
       const jerryx_arg_t mapping[] ={
            jerryx_arg_uint32(&bytedata, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
            };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
                return rv;
            }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");


        if(native_pointer) {
            TwoWire* wire = (TwoWire*)native_pointer;
            return jerry_number(wire->write((uint8_t)bytedata));
        }        
    }
    return jerry_undefined();
} /*js_write*/
//int available();
JS_METHOD(available)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_number(wire->available());
    }
    return jerry_undefined();
} /*js_available*/

//int read();
JS_METHOD(read)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_number(wire->read());
    }
    return jerry_undefined();
} /*js_read*/
//int peek();
JS_METHOD(peek)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_number(wire->peek());
    }
    return jerry_undefined();
} /*js_peek*/
//void flush();
JS_METHOD(flush)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        wire->flush();
    }
    return jerry_undefined();
} /*js_flush*/
JS_METHOD(slaveWrite)
{
    JERRYX_UNUSED(call_info_p);
    char buf[32];
    int len;
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);


    const jerryx_arg_t mapping[] ={
        jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
            return rv;
        }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");


    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        return jerry_number(wire->slaveWrite((const uint8_t *)buf,(size_t)len));
    }
    return jerry_undefined();
} /*js_slaveWrite*/

static jerry_value_t  onReceive_cb_func = jerry_undefined();
void onReceive_cb(int inbyte){
    // *setup argp argc
  jerry_value_t args[] = {
    jerry_number (inbyte),
  };
  if (!jerry_value_is_undefined(onReceive_cb_func)) 
    jerry_value_t result_val = jerry_call (onReceive_cb_func, jerry_undefined(), args, 1);
    return;
}
// void onReceive( void (*)(int) );
JS_METHOD(onReceive){ //1
  JERRYX_UNUSED(call_info_p);
  jerry_value_t callback_fn = 0; //2

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_function(&callback_fn, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX((args_cnt != 1) , "Wrong arguments count");  //4
  onReceive_cb_func  = jerry_value_copy (callback_fn); // release at disconnect
    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        wire->onReceive(onReceive_cb);
    }
  return jerry_undefined();
} /*js_onReceive*/ //6

static jerry_value_t  onRequest_cb_func = jerry_undefined();
void onRequest_cb(){
    // *setup argp argc
  if (!jerry_value_is_undefined(onRequest_cb_func)) 
    jerry_value_t result_val = jerry_call (onRequest_cb_func, jerry_undefined(), NULL, 0);
    return;
}
    // void onRequest( void (*)() );
JS_METHOD(onRequest){ //1
  JERRYX_UNUSED(call_info_p);
  jerry_value_t callback_fn = 0; //2

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &wire_info);

  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_function(&callback_fn, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX((args_cnt != 1) , "Wrong arguments count");  //4
     onRequest_cb_func  = jerry_value_copy (callback_fn); // release at disconnect
    if(native_pointer) {
        TwoWire* wire = (TwoWire*)native_pointer;
        wire->onRequest(onRequest_cb);
    }
  return jerry_undefined();
} /*js_onRequest*/ //6
    // void onRequest( void (*)(void) );

/* destroy */
void wire_free_callback(void* native_p, jerry_object_native_info_t *info_p)
{
    WSDEBUG_TPRINTF("[Wire] delete %x\n",native_p);
    TwoWire* wire = (TwoWire*)native_p;
    if (!jerry_value_is_undefined(onReceive_cb_func)) 
        jerry_value_free(onReceive_cb_func);
    if (!jerry_value_is_undefined(onRequest_cb_func)) 
        jerry_value_free(onRequest_cb_func);    
    delete(wire);
}


// TwoWire::TwoWire(uint8_t bus_num)
JS_METHOD(TwoWire) // 1constructor
{
  int32_t   busnum; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&busnum, JERRYX_ARG_CEIL, JERRYX_ARG_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  

  TwoWire* wire = new TwoWire(busnum); //4
  WSDEBUG_TPRINTF("[WIRE] new %x\n",wire);
  jerry_value_t js_wireobj = jerry_object(); 
  jerry_object_set_native_ptr(js_wireobj, &wire_info , wire );
      
      // Register initialization function
      jerryx_property_entry props[] ={ //2 methods
        JERRYX_PROPERTY_STRING_SZ ("id", "TwoWire"),
        JERRYX_PROPERTY_FUNCTION ("setPins", js_setPins),
        JERRYX_PROPERTY_FUNCTION ("begin", js_begin),
        JERRYX_PROPERTY_FUNCTION ("setBufferSize", js_setBufferSize),
        JERRYX_PROPERTY_FUNCTION ("setTimeOut", js_setTimeOut),
        JERRYX_PROPERTY_FUNCTION ("getTimeOut", js_getTimeOut),
        JERRYX_PROPERTY_FUNCTION ("setClock", js_setClock),
        JERRYX_PROPERTY_FUNCTION ("getClock", js_getClock),
        JERRYX_PROPERTY_FUNCTION ("beginTransmission", js_beginTransmission),
        JERRYX_PROPERTY_FUNCTION ("endTransmission", js_endTransmission),
        JERRYX_PROPERTY_FUNCTION ("requestFrom", js_requestFrom),
        JERRYX_PROPERTY_FUNCTION ("write", js_write),
        JERRYX_PROPERTY_FUNCTION ("available", js_available),
        JERRYX_PROPERTY_FUNCTION ("read", js_read),
        JERRYX_PROPERTY_FUNCTION ("peek", js_peek),
        JERRYX_PROPERTY_FUNCTION ("flush", js_flush),
        JERRYX_PROPERTY_FUNCTION ("slaveWrite", js_slaveWrite),
        JERRYX_PROPERTY_FUNCTION ("onReceive", js_onReceive),
        JERRYX_PROPERTY_FUNCTION ("onRequest", js_onRequest),
        JERRYX_PROPERTY_LIST_END(),
      };
      jerryx_register_result reg = jerryx_set_properties (js_wireobj, props);
      if (jerry_value_is_exception (reg.result))
        {
          WSDEBUG_TPRINTF("Only registered %d properties\r\n", reg.registered);
          /* clean up not registered property values */
          jerryx_release_property_entry (props, reg);
          jerry_value_free (reg.result);
          return false;
          /* clean up the error */
        }
    
    return js_wireobj;
} /*js_TwoWire*/
bool js_wire_classobj_wraper(){  //1 a)modified func name and b) define in .h c) call by jswwrap_tc

  jerryx_register_global ("TwoWire", js_TwoWire);

  return true;
}
#endif //_LIB_WIRE_
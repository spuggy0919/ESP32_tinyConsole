//C++ wrapper auto generator jswmgen.js ver.0.9.0
//https://github.com/spuggy0919/jswrapper.git
//You should examine and fix the code to meet your requirements!
//Json:./json/TwoWire1.json
//File:./jswrapper/jsw_wire.cpp
//className:TwoWire
//classObj:twowire
//constructorPrototype:TwoWire::TwoWire(uint8_t bus_num)
//prototypesArray:
//   bool begin()
//   bool begin(uint8_t address)
//   bool begin(int sda, int scl, uint32_t frequency)
//   bool end()
//   bool setClock(uint32_t freq)
//   void beginTransmission(uint8_t address)
//   uint8_t endTransmission(bool stopBit)
//   uint8_t endTransmission()
//   size_t requestFrom(uint8_t address, size_t len, bool stopBit)
//   size_t requestFrom(uint8_t address, size_t len)
//   void onReceive(funcptr cb1)
//   void onRequest(funcptr cb2)
//   bool setPins(int sda, int scl)
//   size_t setBufferSize(size_t bSize)
//   void setTimeOut(uint16_t timeOutMillis)
//   uint16_t getTimeOut()
//   uint32_t getClock()
//   size_t write(uint8_t c)
//   size_t write(uint8_t* buf , size_t len)
//   int available()
//   int read()
//   int peek()
//   void flush()
//   bool slavebegin(uint8_t slaveAddr, int sda, int scl, uint32_t frequency)
//   size_t slaveWrite(uint8_t* buf , size_t len)
#include "jswrap_tc.h"
#ifdef _LANG_JERRYSCRIPT_ 
#ifdef _LIB_TWOWIRE_
/* **HOW 1** simple select copy & paste **/
/* **HOW 2** or using process.argv[2] for output file, then dl file**/
/* **WARNNING** if tab position is wrong, means error found**/
/* **TODO** Include your class declaration here **/
#include "Wire.h"
void twowire_free_callback(void* native_p, jerry_object_native_info_t *info_p);
const static jerry_object_native_info_t twowire_info = {
    .free_cb = twowire_free_callback
};
// merge three into one begin() with optional args // 1
//bool begin()
//bool begin(uint8_t address)
//bool begin(int sda, int scl, uint32_t frequency=0)
JS_METHOD(twowire_begin)
{
    JERRYX_UNUSED(call_info_p);
    int sda;  // optional alias as address //2
    int scl;  // optional
    uint32_t frequency = 0; // optional =0 for default assign

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int32(&sda, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
        jerryx_arg_int32(&scl, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL), // optional //3
        jerryx_arg_uint32(&frequency, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL), //optional
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count"); // 4 disable

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        if (args_cnt==0) // 5 check args_cnt to call different methods
            return jerry_boolean(twowire->begin());
        else  if (args_cnt==1)
            return jerry_boolean(twowire->begin((uint8_t) sda)); // bool begin(uint8_t address)
        else  if (args_cnt==2||args_cnt==3)
            return jerry_boolean(twowire->begin((int) sda,(int) scl,(uint32_t) frequency));
    }
    return jerry_undefined();
} /*js_twowire_begin*/
//bool end()
JS_METHOD(twowire_end)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_boolean(twowire->end());
    }
    return jerry_undefined();
} /*js_twowire_end*/
//bool setClock(uint32_t freq)
JS_METHOD(twowire_setClock)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t freq;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&freq, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_boolean(twowire->setClock((uint32_t) freq));
    }
    return jerry_undefined();
} /*js_twowire_setClock*/
//void beginTransmission(uint8_t address)
JS_METHOD(twowire_beginTransmission)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t address;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&address, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        twowire->beginTransmission((uint8_t) address);
    }
    return jerry_undefined();
} /*js_twowire_beginTransmission*/
//uint8_t endTransmission(bool stopBit)
//uint8_t endTransmission()
JS_METHOD(twowire_endTransmission)
{
    JERRYX_UNUSED(call_info_p);
    bool stopBit; // optional

    const jerryx_arg_t mapping[] ={
        jerryx_arg_boolean(&stopBit,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        if (args_cnt==1 )
            return jerry_number(twowire->endTransmission((bool) stopBit));
        else if (args_cnt==0)
            return jerry_number(twowire->endTransmission());
    }
    return jerry_undefined();
} /*js_twowire_endTransmission*/
//size_t requestFrom(uint8_t address, size_t len, bool stopBit)
//size_t requestFrom(uint8_t address, size_t len)
JS_METHOD(twowire_requestFrom)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t address;
    size_t len;
    bool stopBit; // optional

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&address, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_boolean(&stopBit,  JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        if (args_cnt==3 )
            return jerry_number(twowire->requestFrom((uint8_t) address,(size_t) len,(bool) stopBit));
        else if (args_cnt==2 )
            return jerry_number(twowire->requestFrom((uint8_t) address,(size_t) len));
    }
    return jerry_undefined();
} /*js_twowire_requestFrom*/
//void onReceive(funcptr cb0)
typedef void (*TWOWIRE_CALLBACK0)(int); // define call back type 
static jerry_value_t jscallback0 = NULL;  // maybe need allocate for each TwoWire instance
typedef void (*TWOWIRE_CALLBACK1)();
static jerry_value_t jscallback1 = NULL;  // maybe need allocate for each TwoWire instance
void twowire_callback0(){
        jerry_value_t result_val = jerry_call (jscallback0, jerry_undefined(), NULL, 0);
        jerry_value_free (result_val);    
}
void twowire_callback1(int data){
      jerry_value_t args[] = {
           jerry_number (data),
      };
        jerry_value_t result_val = jerry_call (jscallback0, jerry_undefined(), args, 1);
        jerry_value_free (result_val);    
        jerry_value_free (args[0]);    
        // jerry_value_free (args);    // to be confirm???
}
//void onReceive(funcptr cb0)
JS_METHOD(twowire_onReceive)
{
    JERRYX_UNUSED(call_info_p);
    jerry_value_t cb0;

    const jerryx_arg_t mapping[] ={
        /* **TODO** need to define your c callback to use jerry_call */
        jerryx_arg_function(&cb0, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        jscallback0 = jerry_value_copy (cb0); // release at disconnect
        twowire->onReceive((TWOWIRE_CALLBACK0) twowire_callback0);
    }
    return jerry_undefined();
} /*js_twowire_onReceive*/
//void onRequest(funcptr cb1)
JS_METHOD(twowire_onRequest)
{
    JERRYX_UNUSED(call_info_p);
    jerry_value_t cb1;

    const jerryx_arg_t mapping[] ={
        /* **TODO** need to define your c callback to use jerry_call */
        jerryx_arg_function(&cb1, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        jscallback1 = jerry_value_copy (cb1); // release at disconnect
        twowire->onRequest((TWOWIRE_CALLBACK1) twowire_callback1);
    }
    return jerry_undefined();
} /*js_twowire_onRequest*/
//bool setPins(int sda, int scl)
JS_METHOD(twowire_setPins)
{
    JERRYX_UNUSED(call_info_p);
    int sda;
    int scl;

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
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_boolean(twowire->setPins((int) sda,(int) scl));
    }
    return jerry_undefined();
} /*js_twowire_setPins*/
//size_t setBufferSize(size_t bSize)
JS_METHOD(twowire_setBufferSize)
{
    JERRYX_UNUSED(call_info_p);
    size_t bSize;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&bSize, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_number(twowire->setBufferSize((size_t) bSize));
    }
    return jerry_undefined();
} /*js_twowire_setBufferSize*/
//void setTimeOut(uint16_t timeOutMillis)
JS_METHOD(twowire_setTimeOut)
{
    JERRYX_UNUSED(call_info_p);
    uint16_t timeOutMillis;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint16(&timeOutMillis, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        twowire->setTimeOut((uint16_t) timeOutMillis);
    }
    return jerry_undefined();
} /*js_twowire_setTimeOut*/
//uint16_t getTimeOut()
JS_METHOD(twowire_getTimeOut)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_number(twowire->getTimeOut());
    }
    return jerry_undefined();
} /*js_twowire_getTimeOut*/
//uint32_t getClock()
JS_METHOD(twowire_getClock)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_number(twowire->getClock());
    }
    return jerry_undefined();
} /*js_twowire_getClock*/
//size_t write(uint8_t c)
//size_t write(uint8_t* buf , size_t len)
JS_METHOD(twowire_write)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t c; // for one arg

    uint8_t* buf; // for two args
    size_t len;
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);
    if (args_cnt==1){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_uint8(&c, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");
        if(native_pointer) {
            TwoWire* twowire = (TwoWire*)native_pointer;
            return jerry_number(twowire->write((uint8_t) c));
        }
    }else if (args_cnt==2){
        const jerryx_arg_t mapping[] ={
            jerryx_arg_arraybuffer(&buf,   JERRYX_ARG_REQUIRED),
            jerryx_arg_uint32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        };
        const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
        if (jerry_value_is_exception(rv)){
            return rv;
        }
        JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");
        if(native_pointer) {
            TwoWire* twowire = (TwoWire*)native_pointer;
            return jerry_number(twowire->write((uint8_t*) buf,(size_t) len));
        }
    }

    return jerry_undefined();
} /*js_twowire_write*/
//int available()
JS_METHOD(twowire_available)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_number(twowire->available());
    }
    return jerry_undefined();
} /*js_twowire_available*/
//int read()
JS_METHOD(twowire_read)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_number(twowire->read());
    }
    return jerry_undefined();
} /*js_twowire_read*/
//int peek()
JS_METHOD(twowire_peek)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_number(twowire->peek());
    }
    return jerry_undefined();
} /*js_twowire_peek*/
//void flush()
JS_METHOD(twowire_flush)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        twowire->flush();
    }
    return jerry_undefined();
} /*js_twowire_flush*/
//bool slavebegin(uint8_t slaveAddr, int sda, int scl, uint32_t frequency)
JS_METHOD(twowire_slavebegin)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t slaveAddr;
    int sda;
    int scl;
    uint32_t frequency;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&slaveAddr, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int32(&sda, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int32(&scl, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&frequency, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_boolean(twowire->begin((uint8_t) slaveAddr,(int) sda,(int) scl,(uint32_t) frequency));
    }
    return jerry_undefined();
} /*js_twowire_slavebegin*/
//size_t slaveWrite(uint8_t* buf , size_t len)
JS_METHOD(twowire_slaveWrite)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t* buf;
    size_t len;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_arraybuffer(&buf,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&len, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &twowire_info);

    if(native_pointer) {
        TwoWire* twowire = (TwoWire*)native_pointer;
        return jerry_number(twowire->slaveWrite((uint8_t*) buf,(size_t) len));
    }
    return jerry_undefined();
} /*js_twowire_slaveWrite*/
/* ~TwoWire twowire destroy */
void twowire_free_callback(void* native_p, jerry_object_native_info_t *info_p){
    WSDEBUG_TPRINTF("[TwoWire] delete %x\n",native_p);
    TwoWire* twowire = (TwoWire*)native_p;
    /** **TODO** free your own resource here **/
    /* TODO consider the callback Jerry_value_free*/
    // delete(twowire); use static class object
};
//Verify Methods List
/*js_twowire_begin 3 merge to 1*/
/*js_twowire_end*/
/*js_twowire_setClock*/
/*js_twowire_beginTransmission*/
/*js_twowire_endTransmission 2 merge to 1*/
/*js_twowire_requestFrom 2 merge to 1*/
/*js_twowire_onReceive*/
/*js_twowire_onRequest*/
/*js_twowire_setPins*/
/*js_twowire_setBufferSize*/
/*js_twowire_setTimeOut*/
/*js_twowire_getTimeOut*/
/*js_twowire_getClock*/
/*js_twowire_write 2 merge to 1*/
/*js_twowire_write*/
/*js_twowire_available*/
/*js_twowire_read*/
/*js_twowire_peek*/
/*js_twowire_flush*/
/*js_twowire_slavebegin*/
/*js_twowire_slaveWrite*/
//TwoWire::TwoWire(uint8_t bus_num)
JS_METHOD(twowire_TwoWire)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t bus_num;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&bus_num, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");
    TwoWire* twowire = (bus_num==0)? &Wire : &Wire1; //4 use static class object
    WSDEBUG_TPRINTF("[TwoWire] new %x\n",twowire);
    jerry_value_t js_twowireobj = jerry_object();
    jerry_object_set_native_ptr(js_twowireobj, &twowire_info , twowire );
    // Register initialization function
    jerryx_property_entry props[] ={ //5 methods
        JERRYX_PROPERTY_STRING_SZ ("id", "TwoWire"),
        JERRYX_PROPERTY_FUNCTION ("begin", js_twowire_begin),
        // JERRYX_PROPERTY_FUNCTION ("begin", js_twowire_begin),
        // JERRYX_PROPERTY_FUNCTION ("begin", js_twowire_begin), merge
        JERRYX_PROPERTY_FUNCTION ("end", js_twowire_end),
        JERRYX_PROPERTY_FUNCTION ("setClock", js_twowire_setClock),
        JERRYX_PROPERTY_FUNCTION ("beginTransmission", js_twowire_beginTransmission),
        JERRYX_PROPERTY_FUNCTION ("endTransmission", js_twowire_endTransmission),
        // JERRYX_PROPERTY_FUNCTION ("endTransmission", js_twowire_endTransmission), merge
        JERRYX_PROPERTY_FUNCTION ("requestFrom", js_twowire_requestFrom),
        // JERRYX_PROPERTY_FUNCTION ("requestFrom", js_twowire_requestFrom), merge
        JERRYX_PROPERTY_FUNCTION ("onReceive", js_twowire_onReceive),
        JERRYX_PROPERTY_FUNCTION ("onRequest", js_twowire_onRequest),
        JERRYX_PROPERTY_FUNCTION ("setPins", js_twowire_setPins),
        JERRYX_PROPERTY_FUNCTION ("setBufferSize", js_twowire_setBufferSize),
        JERRYX_PROPERTY_FUNCTION ("setTimeOut", js_twowire_setTimeOut),
        JERRYX_PROPERTY_FUNCTION ("getTimeOut", js_twowire_getTimeOut),
        JERRYX_PROPERTY_FUNCTION ("getClock", js_twowire_getClock),
        JERRYX_PROPERTY_FUNCTION ("write", js_twowire_write),
        // JERRYX_PROPERTY_FUNCTION ("write", js_twowire_write), merge
        JERRYX_PROPERTY_FUNCTION ("available", js_twowire_available),
        JERRYX_PROPERTY_FUNCTION ("read", js_twowire_read),
        JERRYX_PROPERTY_FUNCTION ("peek", js_twowire_peek),
        JERRYX_PROPERTY_FUNCTION ("flush", js_twowire_flush),
        JERRYX_PROPERTY_FUNCTION ("slavebegin", js_twowire_slavebegin),
        JERRYX_PROPERTY_FUNCTION ("slaveWrite", js_twowire_slaveWrite),
        JERRYX_PROPERTY_LIST_END(),
    };
    jerryx_register_result reg = jerryx_set_properties (js_twowireobj, props);
    if (jerry_value_is_exception (reg.result)){
        WSDEBUG_TPRINTF("Only registered %d properties\r\n", reg.registered);
        /* clean up not registered property values */
        jerryx_release_property_entry (props, reg);
        jerry_value_free (reg.result);
        return jerry_undefined();
        /* clean up the error */
    };
    return js_twowireobj;
} /*js_twowire_TwoWire*/
bool js_twowire_classobj_wrapper(){  
//1 a)modified func name and b) define in .h c) call by jswwrap_tc
    return jerryx_register_global ("TwoWire", js_twowire_TwoWire);
};
#endif //_LIB_TWOWIRE_
#endif // _LANG_JERRYSCRIPT_ Congraturation Done!

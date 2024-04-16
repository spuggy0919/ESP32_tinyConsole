//Json:/js/jswmgen/esp.json
//File:function () { /* ecmascript */ }
//className:EspClass
//classObj:espclass
//constructorPrototype:EspClass::EspClass();
//prototypesArray:
//   void restart();
//   uint32_t getHeapSize();
//   uint32_t getFreeHeap();
//   uint32_t getMinFreeHeap();
//   uint32_t getMaxAllocHeap();
//   uint32_t getPsramSize();
//   uint32_t getFreePsram();
//   uint32_t getMinFreePsram();
//   uint32_t getMaxAllocPsram();
//   uint16_t getChipRevision();
//   uint8_t getChipCores();
//   uint32_t getCpuFreqMHz();
//   uint32_t getCycleCount();
//   void deepSleep(uint32_t time_us);
//   uint32_t getFlashChipSize();
//   uint32_t getFlashChipSpeed();
//   uint8_t getFlashChipMode();
//   uint32_t magicFlashChipSize(uint8_t byte);
//   uint32_t magicFlashChipSpeed(uint8_t byte);
//   uint8_t magicFlashChipMode(uint8_t byte);
//   uint32_t getSketchSize();
//   uint8_t* getSketchMD5();
//   uint32_t getFreeSketchSpace();
//   bool flashEraseSector(uint32_t sector);
//   bool flashWrite(uint32_t offset, uint32_t* data, size_t size);
//   bool flashRead(uint32_t offset, uint32_t* data, size_t size);
//   bool partitionEraseRange(uint8_t* partition, uint32_t offset, size_t size);
//   bool partitionWrite(uint8_t* partition, uint32_t offset, uint32_t* data, size_t size);
//   bool partitionRead(uint8_t* partition, uint32_t offset, uint32_t* data, size_t size);
//   uint32_t getEfuseMac();
#include "jswrap_tc.h"
#ifdef _LANG_JERRYSCRIPT_ 
#ifdef _LIB_ESPCLASS_
/* **HOW 1** simple select copy & paste **/
/* **HOW 2** or using process.argv[2] for output file, then dl file**/
/* **WARNNING** if tab position is wrong, means error found**/
/* **TODO** Include your class declaration here **/
#include "esp.h"
void espclass_free_callback(void* native_p, jerry_object_native_info_t *info_p);
const static jerry_object_native_info_t espclass_info = {
    .free_cb = espclass_free_callback
};
//void restart();
JS_METHOD(espclass_restart)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        espclass->restart();
    }
    return jerry_undefined();
} /*js_espclass_restart*/
//uint32_t getHeapSize();
JS_METHOD(espclass_getHeapSize)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getHeapSize());
    }
    return jerry_undefined();
} /*js_espclass_getHeapSize*/
//uint32_t getFreeHeap();
JS_METHOD(espclass_getFreeHeap)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getFreeHeap());
    }
    return jerry_undefined();
} /*js_espclass_getFreeHeap*/
//uint32_t getMinFreeHeap();
JS_METHOD(espclass_getMinFreeHeap)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getMinFreeHeap());
    }
    return jerry_undefined();
} /*js_espclass_getMinFreeHeap*/
//uint32_t getMaxAllocHeap();
JS_METHOD(espclass_getMaxAllocHeap)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getMaxAllocHeap());
    }
    return jerry_undefined();
} /*js_espclass_getMaxAllocHeap*/
//uint32_t getPsramSize();
JS_METHOD(espclass_getPsramSize)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getPsramSize());
    }
    return jerry_undefined();
} /*js_espclass_getPsramSize*/
//uint32_t getFreePsram();
JS_METHOD(espclass_getFreePsram)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getFreePsram());
    }
    return jerry_undefined();
} /*js_espclass_getFreePsram*/
//uint32_t getMinFreePsram();
JS_METHOD(espclass_getMinFreePsram)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getMinFreePsram());
    }
    return jerry_undefined();
} /*js_espclass_getMinFreePsram*/
//uint32_t getMaxAllocPsram();
JS_METHOD(espclass_getMaxAllocPsram)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getMaxAllocPsram());
    }
    return jerry_undefined();
} /*js_espclass_getMaxAllocPsram*/
//uint16_t getChipRevision();
JS_METHOD(espclass_getChipRevision)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getChipRevision());
    }
    return jerry_undefined();
} /*js_espclass_getChipRevision*/
//uint8_t getChipCores();
JS_METHOD(espclass_getChipCores)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getChipCores());
    }
    return jerry_undefined();
} /*js_espclass_getChipCores*/
//uint32_t getCpuFreqMHz();
JS_METHOD(espclass_getCpuFreqMHz)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getCpuFreqMHz());
    }
    return jerry_undefined();
} /*js_espclass_getCpuFreqMHz*/
//uint32_t getCycleCount();
JS_METHOD(espclass_getCycleCount)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getCycleCount());
    }
    return jerry_undefined();
} /*js_espclass_getCycleCount*/
//void deepSleep(uint32_t time_us);
JS_METHOD(espclass_deepSleep)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t time_us_lo;
    uint32_t time_us_hi;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&time_us_lo, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&time_us_hi, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        espclass->deepSleep((uint64_t) time_us_lo||((uint64_t) time_us_hi<<32));
    }
    return jerry_undefined();
} /*js_espclass_deepSleep*/
//uint32_t getFlashChipSize();
JS_METHOD(espclass_getFlashChipSize)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getFlashChipSize());
    }
    return jerry_undefined();
} /*js_espclass_getFlashChipSize*/
//uint32_t getFlashChipSpeed();
JS_METHOD(espclass_getFlashChipSpeed)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getFlashChipSpeed());
    }
    return jerry_undefined();
} /*js_espclass_getFlashChipSpeed*/
//uint8_t getFlashChipMode();
JS_METHOD(espclass_getFlashChipMode)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getFlashChipMode());
    }
    return jerry_undefined();
} /*js_espclass_getFlashChipMode*/
//uint32_t magicFlashChipSize(uint8_t byte);
JS_METHOD(espclass_magicFlashChipSize)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t byte;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&byte, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->magicFlashChipSize((uint8_t) byte));
    }
    return jerry_undefined();
} /*js_espclass_magicFlashChipSize*/
//uint32_t magicFlashChipSpeed(uint8_t byte);
JS_METHOD(espclass_magicFlashChipSpeed)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t byte;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&byte, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->magicFlashChipSpeed((uint8_t) byte));
    }
    return jerry_undefined();
} /*js_espclass_magicFlashChipSpeed*/
//uint8_t magicFlashChipMode(uint8_t byte);
JS_METHOD(espclass_magicFlashChipMode)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t byte;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint8(&byte, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->magicFlashChipMode((uint8_t) byte));
    }
    return jerry_undefined();
} /*js_espclass_magicFlashChipMode*/
//uint32_t getSketchSize();
JS_METHOD(espclass_getSketchSize)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getSketchSize());
    }
    return jerry_undefined();
} /*js_espclass_getSketchSize*/
//uint8_t* getSketchMD5();
JS_METHOD(espclass_getSketchMD5)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_string_sz(espclass->getSketchMD5().c_str());
    }
    return jerry_undefined();
} /*js_espclass_getSketchMD5*/
//uint32_t getFreeSketchSpace();
JS_METHOD(espclass_getFreeSketchSpace)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_number(espclass->getFreeSketchSpace());
    }
    return jerry_undefined();
} /*js_espclass_getFreeSketchSpace*/
//bool flashEraseSector(uint32_t sector);
JS_METHOD(espclass_flashEraseSector)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t sector;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&sector, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_boolean(espclass->flashEraseSector((uint32_t) sector));
    }
    return jerry_undefined();
} /*js_espclass_flashEraseSector*/
//bool flashWrite(uint32_t offset, uint32_t* data, size_t size);
JS_METHOD(espclass_flashWrite)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t offset;
    uint8_t* data;
    size_t size;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&offset, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_arraybuffer(&data,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&size, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_boolean(espclass->flashWrite((uint32_t) offset,(uint32_t*) data,(size_t) size));
    }
    return jerry_undefined();
} /*js_espclass_flashWrite*/
//bool flashRead(uint32_t offset, uint32_t* data, size_t size);
JS_METHOD(espclass_flashRead)
{
    JERRYX_UNUSED(call_info_p);
    uint32_t offset;
    uint8_t* data;
    size_t size;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_uint32(&offset, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_arraybuffer(&data,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&size, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_boolean(espclass->flashRead((uint32_t) offset,(uint32_t*) data,(size_t) size));
    }
    return jerry_undefined();
} /*js_espclass_flashRead*/
//bool partitionEraseRange(uint8_t* partition, uint32_t offset, size_t size);
JS_METHOD(espclass_partitionEraseRange)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t* partition;
    uint32_t offset;
    size_t size;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_arraybuffer(&partition,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&offset, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&size, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 3, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_boolean(espclass->partitionEraseRange((const esp_partition_t *) partition,(uint32_t) offset,(size_t) size));
    }
    return jerry_undefined();
} /*js_espclass_partitionEraseRange*/
//bool partitionWrite(uint8_t* partition, uint32_t offset, uint32_t* data, size_t size);
JS_METHOD(espclass_partitionWrite)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t* partition;
    uint32_t offset;
    uint8_t* data;
    size_t size;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_arraybuffer(&partition,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&offset, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_arraybuffer(&data,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&size, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_boolean(espclass->partitionWrite((const esp_partition_t *) partition,(uint32_t) offset,(uint32_t*) data,(size_t) size));
    }
    return jerry_undefined();
} /*js_espclass_partitionWrite*/
//bool partitionRead(uint8_t* partition, uint32_t offset, uint32_t* data, size_t size);
JS_METHOD(espclass_partitionRead)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t* partition;
    uint32_t offset;
    uint8_t* data;
    size_t size;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_arraybuffer(&partition,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&offset, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_arraybuffer(&data,   JERRYX_ARG_REQUIRED),
        jerryx_arg_uint32(&size, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 4, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        return jerry_boolean(espclass->partitionRead((const esp_partition_t *) partition,(uint32_t) offset,(uint32_t*) data,(size_t) size));
    }
    return jerry_undefined();
} /*js_espclass_partitionRead*/
//uint32_t getEfuseMac(); // return 8 bytes arraybuf call by reference
JS_METHOD(espclass_getEfuseMac)
{
    JERRYX_UNUSED(call_info_p);
    uint8_t* mac;

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &espclass_info);
    const jerryx_arg_t mapping[] ={
        jerryx_arg_arraybuffer(&mac,   JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");

    if(native_pointer) {
        EspClass* espclass = (EspClass*)native_pointer;
        uint64_t efusMac= espclass->getEfuseMac();
        uint8_t *p=(uint8_t *)&efusMac;
        for(int i=0;i<8;i++) {
            mac[i] = *p++;
            WSDEBUG_TPRINTF("[ESP]getEfuseMac is arraybuf%2x\n",mac[i]);
        }
    }
    return jerry_undefined();
} /*js_espclass_getEfuseMac*/
/* ~EspClass espclass destroy */
void espclass_free_callback(void* native_p, jerry_object_native_info_t *info_p){
    WSDEBUG_TPRINTF("[EspClass] delete %x\n",native_p);
    EspClass* espclass = (EspClass*)native_p;
    /** **TODO** free your own resource here **/
  //  delete(espclass); using external no need to delete
};
//Verify Methods List
/*js_espclass_restart*/
/*js_espclass_getHeapSize*/
/*js_espclass_getFreeHeap*/
/*js_espclass_getMinFreeHeap*/
/*js_espclass_getMaxAllocHeap*/
/*js_espclass_getPsramSize*/
/*js_espclass_getFreePsram*/
/*js_espclass_getMinFreePsram*/
/*js_espclass_getMaxAllocPsram*/
/*js_espclass_getChipRevision*/
/*js_espclass_getChipCores*/
/*js_espclass_getCpuFreqMHz*/
/*js_espclass_getCycleCount*/
/*js_espclass_deepSleep*/
/*js_espclass_getFlashChipSize*/
/*js_espclass_getFlashChipSpeed*/
/*js_espclass_getFlashChipMode*/
/*js_espclass_magicFlashChipSize*/
/*js_espclass_magicFlashChipSpeed*/
/*js_espclass_magicFlashChipMode*/
/*js_espclass_getSketchSize*/
/*js_espclass_getSketchMD5*/
/*js_espclass_getFreeSketchSpace*/
/*js_espclass_flashEraseSector*/
/*js_espclass_flashWrite*/
/*js_espclass_flashRead*/
/*js_espclass_partitionEraseRange*/
/*js_espclass_partitionWrite*/
/*js_espclass_partitionRead*/
/*js_espclass_getEfuseMac*/
//EspClass::EspClass();
JS_METHOD(espclass_EspClass)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
    EspClass* espclass = &ESP; //4
    WSDEBUG_TPRINTF("[EspClass] new %x\n",espclass);
    jerry_value_t js_espclassobj = jerry_object();
    jerry_object_set_native_ptr(js_espclassobj, &espclass_info , espclass );
    // Register initialization function
    jerryx_property_entry props[] ={ //5 methods
        JERRYX_PROPERTY_STRING_SZ ("id", "ESP"),
        JERRYX_PROPERTY_FUNCTION ("restart", js_espclass_restart),
        JERRYX_PROPERTY_FUNCTION ("getHeapSize", js_espclass_getHeapSize),
        JERRYX_PROPERTY_FUNCTION ("getFreeHeap", js_espclass_getFreeHeap),
        JERRYX_PROPERTY_FUNCTION ("getMinFreeHeap", js_espclass_getMinFreeHeap),
        JERRYX_PROPERTY_FUNCTION ("getMaxAllocHeap", js_espclass_getMaxAllocHeap),
        JERRYX_PROPERTY_FUNCTION ("getPsramSize", js_espclass_getPsramSize),
        JERRYX_PROPERTY_FUNCTION ("getFreePsram", js_espclass_getFreePsram),
        JERRYX_PROPERTY_FUNCTION ("getMinFreePsram", js_espclass_getMinFreePsram),
        JERRYX_PROPERTY_FUNCTION ("getMaxAllocPsram", js_espclass_getMaxAllocPsram),
        JERRYX_PROPERTY_FUNCTION ("getChipRevision", js_espclass_getChipRevision),
        JERRYX_PROPERTY_FUNCTION ("getChipCores", js_espclass_getChipCores),
        JERRYX_PROPERTY_FUNCTION ("getCpuFreqMHz", js_espclass_getCpuFreqMHz),
        JERRYX_PROPERTY_FUNCTION ("getCycleCount", js_espclass_getCycleCount),
        JERRYX_PROPERTY_FUNCTION ("deepSleep", js_espclass_deepSleep),
        JERRYX_PROPERTY_FUNCTION ("getFlashChipSize", js_espclass_getFlashChipSize),
        JERRYX_PROPERTY_FUNCTION ("getFlashChipSpeed", js_espclass_getFlashChipSpeed),
        JERRYX_PROPERTY_FUNCTION ("getFlashChipMode", js_espclass_getFlashChipMode),
        JERRYX_PROPERTY_FUNCTION ("magicFlashChipSize", js_espclass_magicFlashChipSize),
        JERRYX_PROPERTY_FUNCTION ("magicFlashChipSpeed", js_espclass_magicFlashChipSpeed),
        JERRYX_PROPERTY_FUNCTION ("magicFlashChipMode", js_espclass_magicFlashChipMode),
        JERRYX_PROPERTY_FUNCTION ("getSketchSize", js_espclass_getSketchSize),
        JERRYX_PROPERTY_FUNCTION ("getSketchMD5", js_espclass_getSketchMD5),
        JERRYX_PROPERTY_FUNCTION ("getFreeSketchSpace", js_espclass_getFreeSketchSpace),
        JERRYX_PROPERTY_FUNCTION ("flashEraseSector", js_espclass_flashEraseSector),
        JERRYX_PROPERTY_FUNCTION ("flashWrite", js_espclass_flashWrite),
        JERRYX_PROPERTY_FUNCTION ("flashRead", js_espclass_flashRead),
        JERRYX_PROPERTY_FUNCTION ("partitionEraseRange", js_espclass_partitionEraseRange),
        JERRYX_PROPERTY_FUNCTION ("partitionWrite", js_espclass_partitionWrite),
        JERRYX_PROPERTY_FUNCTION ("partitionRead", js_espclass_partitionRead),
        JERRYX_PROPERTY_FUNCTION ("getEfuseMac", js_espclass_getEfuseMac),
        JERRYX_PROPERTY_LIST_END(),
    };
    jerryx_register_result reg = jerryx_set_properties (js_espclassobj, props);
    if (jerry_value_is_exception (reg.result)){
        WSDEBUG_TPRINTF("Only registered %d properties\r\n", reg.registered);
        /* clean up not registered property values */
        jerryx_release_property_entry (props, reg);
        jerry_value_free (reg.result);
        return jerry_undefined();
        /* clean up the error */
    };
    return js_espclassobj;
} /*js_espclass_EspClass*/
bool js_espclass_classobj_wrapper(){  
//1 a)modified func name and b) define in .h c) call by jswwrap_tc
    return jerryx_register_global ("EspClass", js_espclass_EspClass);
};
#endif //_LIB_ESPCLASS_
#endif // _LANG_JERRYSCRIPT_ Congraturation Done!

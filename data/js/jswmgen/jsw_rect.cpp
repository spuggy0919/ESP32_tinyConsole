//Json:/js/jswmgen/rectangle.json
//File:function () { /* ecmascript */ }
//className:Rectangle
//classObj:rectangle
//constructorPrototype:Rectangle::Rectangle(int length,int width);
//prototypesArray:
//   int getSize();
//   int getLength();
//   int getWidth();
#include "jswrap_tc.h"
#ifdef _LANG_JERRYSCRIPT_ 
#ifdef _LIB_RECTANGLE_
/* **HOW 1** simple select copy & paste **/
/* **HOW 2** or using process.argv[2] for output file, then dl file**/
/* **WARNNING** if tab position is wrong, means error found**/
/* **TODO** Include your class declaration here **/
#include "Rectangle.h"
void rectangle_free_callback(void* native_p, jerry_object_native_info_t *info_p);
const static jerry_object_native_info_t rectangle_info = {
    .free_cb = rectangle_free_callback
};
//int getSize();
JS_METHOD(rectangle_getSize)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &rectangle_info);

    if(native_pointer) {
        Rectangle* rectangle = (Rectangle*)native_pointer;
        return jerry_number(rectangle->getSize());
    }
    return jerry_undefined();
} /*js_rectangle_getSize*/
//int getLength();
JS_METHOD(rectangle_getLength)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &rectangle_info);

    if(native_pointer) {
        Rectangle* rectangle = (Rectangle*)native_pointer;
        return jerry_number(rectangle->getLength());
    }
    return jerry_undefined();
} /*js_rectangle_getLength*/
//int getWidth();
JS_METHOD(rectangle_getWidth)
{
    JERRYX_UNUSED(call_info_p);
    JERRYX_UNUSED(args_p);
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");

    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &rectangle_info);

    if(native_pointer) {
        Rectangle* rectangle = (Rectangle*)native_pointer;
        return jerry_number(rectangle->getWidth());
    }
    return jerry_undefined();
} /*js_rectangle_getWidth*/
/* ~Rectangle rectangle destroy */
void rectangle_free_callback(void* native_p, jerry_object_native_info_t *info_p){
    WSDEBUG_TPRINTF("[Rectangle] delete %x\n",native_p);
    Rectangle* rectangle = (Rectangle*)native_p;
    /** **TODO** free your own resource here **/
    delete(rectangle);
};
//Verify Methods List
/*js_rectangle_getSize*/
/*js_rectangle_getLength*/
/*js_rectangle_getWidth*/
//Rectangle::Rectangle(int length,int width);
JS_METHOD(rectangle_Rectangle)
{
    JERRYX_UNUSED(call_info_p);
    int length;
    int width;

    const jerryx_arg_t mapping[] ={
        jerryx_arg_int32(&length, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
        jerryx_arg_int32(&width, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
    };
    const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
    if (jerry_value_is_exception(rv)){
        return rv;
    }
    JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");
    Rectangle* rectangle = new Rectangle((int) length,(int) width); //4
    WSDEBUG_TPRINTF("[Rectangle] new %x\n",rectangle);
    jerry_value_t js_rectangleobj = jerry_object();
    jerry_object_set_native_ptr(js_rectangleobj, &rectangle_info , rectangle );
    // Register initialization function
    jerryx_property_entry props[] ={ //5 methods
        JERRYX_PROPERTY_STRING_SZ ("id", "Rectangle"),
        JERRYX_PROPERTY_FUNCTION ("getSize", js_rectangle_getSize),
        JERRYX_PROPERTY_FUNCTION ("getLength", js_rectangle_getLength),
        JERRYX_PROPERTY_FUNCTION ("getWidth", js_rectangle_getWidth),
        JERRYX_PROPERTY_LIST_END(),
    };
    jerryx_register_result reg = jerryx_set_properties (js_rectangleobj, props);
    if (jerry_value_is_exception (reg.result)){
        WSDEBUG_TPRINTF("Only registered %d properties\r\n", reg.registered);
        /* clean up not registered property values */
        jerryx_release_property_entry (props, reg);
        jerry_value_free (reg.result);
        return jerry_undefined();
        /* clean up the error */
    };
    return js_rectangleobj;
} /*js_rectangle_Rectangle*/
bool js_rectangle_classobj_wrapper(){  
//1 a)modified func name and b) define in .h c) call by jswwrap_tc
    return jerryx_register_global ("Rectangle", js_rectangle_Rectangle);
};
#endif //_LIB_RECTANGLE_
#endif // _LANG_JERRYSCRIPT_ Congraturation Done!

/*
modified from 

 https://github.com/RT-Thread-packages/jerryscript/blob/master/examples/ImportCplusplus/README.md
 https://github.com/RT-Thread-packages/jerryscript/blob/master/LICENSE

 */

#include "jswrap_tc.h"

#ifdef _LANG_JERRYSCRIPT_
// create a process object, which keep argc, argvList
class Rectangle
{
   private:
        int length;
        int width;
        int *buf=NULL;
   public:
        Rectangle(int length,int width);
        ~Rectangle();
        int getSize();
        int getLength();
        int getWidth();
        int getBuf(int* buf,int len); // TODO try for typedarray
        int *getBufPtr();
        int setBuf(int *buf,int len);
};
#define BUFFSIZE 10
Rectangle::Rectangle(int length,int width)
{
    this->length = length;
    this->width = width;
    this->buf = (int*)calloc(BUFFSIZE,sizeof(int));
}
Rectangle::~Rectangle()
{
    if (this->buf!=NULL) {
        free(this->buf);
    }
}
int Rectangle::getSize()
{
    return (this->length * this->width);
}

int Rectangle::getLength()
{
    return this->length;
}

int Rectangle::getWidth()
{
    return this->width;
}
int Rectangle::getBuf(int *inbuf,int len)
{
    int actual = (len>BUFFSIZE) ? BUFFSIZE:len;
    memcpy(inbuf,this->buf,actual*sizeof(int));
    return actual;
}
int *Rectangle::getBufPtr()
{
    return this->buf;
}
int Rectangle::setBuf(int *outbuf,int len)
{
    int actual = (len>BUFFSIZE) ? BUFFSIZE:len;
    memcpy(this->buf,outbuf,actual*sizeof(int));
    return actual;
}
/* wrapper */
void rectangle_free_callback(void* native_p,jerry_object_native_info_t *info_p)
{
    wsTextPrintf("[Rectangle] delete %x\n",native_p);
    Rectangle* rect = (Rectangle*)native_p;
    delete(rect);
}

const static jerry_object_native_info_t rectangle_info =
{
    .free_cb = rectangle_free_callback
};

JERRYXX_DECLARE_FUNCTION(getSize)
{
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &rectangle_info);
    
    if(native_pointer)
    {
        Rectangle* rectangle = (Rectangle*)native_pointer;
        wsTextPrintf("[Rectangle] %x, Size %d\n",native_pointer,rectangle->getSize());
        return jerry_number(rectangle->getSize()); //2

    }
    return jerry_undefined();
} /*js_js_getSize*/
JERRYXX_DECLARE_FUNCTION(getLength) //1
{
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &rectangle_info);
    
    if(native_pointer)
    {
        Rectangle* rectangle = (Rectangle*)native_pointer;
        wsTextPrintf("[Rectangle] %x, Length %d\n",native_pointer,rectangle->getLength());

        return jerry_number(rectangle->getLength()); //2
    }
    return jerry_undefined();
} /*js_js_getLength*/ //4
JERRYXX_DECLARE_FUNCTION(getWidth)
{
    void *native_pointer = NULL;
    native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &rectangle_info);
    
    if(native_pointer)
    {
        Rectangle* rectangle = (Rectangle*)native_pointer;
        wsTextPrintf("[Rectangle] %x, Width %d\n",native_pointer,rectangle->getWidth());

        return jerry_number(rectangle->getWidth()); //2

    }
    return jerry_undefined();
} /*js_js_getWidth*/

bool
jerry_set_object_native_pointer (const jerry_value_t obj_val,
                                 void *native_p,
                                 const jerry_object_native_info_t *info_p);
JERRYXX_DECLARE_FUNCTION(Rectangle) // constructor
{
  int32_t   length,width; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_int32(&length, JERRYX_ARG_CEIL, JERRYX_ARG_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_int32(&width, JERRYX_ARG_CEIL, JERRYX_ARG_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");  //4
  Rectangle* rectangle = new Rectangle(length,width);
  wsTextPrintf("[Rectangle] new %x\n",rectangle);

  jerry_value_t js_rectobj = jerry_object();
  jerry_object_set_native_ptr(js_rectobj, &rectangle_info , rectangle );
      
      // Register initialization function
      jerryx_property_entry props[] ={ //2 methods
        // JERRYX_PROPERTY_NUMBER ("length", length),
        // JERRYX_PROPERTY_NUMBER ("width", width),
        JERRYX_PROPERTY_FUNCTION ("getSize", js_getSize),
        JERRYX_PROPERTY_FUNCTION ("getLength", js_getLength),
        JERRYX_PROPERTY_FUNCTION ("getWidth", js_getWidth),
        JERRYX_PROPERTY_LIST_END(),
      };
      jerryx_register_result reg = jerryx_set_properties (js_rectobj, props);
      if (jerry_value_is_exception (reg.result))
        {
          wsTextPrintf ("Only registered %d properties\r\n", reg.registered);
          /* clean up not registered property values */
          jerryx_release_property_entry (props, reg);
          jerry_value_free (reg.result);
          return false;
          /* clean up the error */
        }
    
    return js_rectobj;
} /*js_Rectangle*/
/*************/



bool js_cobj_classobj_wraper(){  //1 a)modified func name and b) define in .h c) call by jswwrap_tc

  jerryx_register_global ("Rectangle", js_Rectangle);

  return true;
}
#endif
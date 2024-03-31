#include "jerryscript.h"
#include "jerryscript-ext.h"

struct my_struct
{
  const char *msg;
} my_struct;

/**
 * Get a string from a native object
 */
static jerry_value_t
get_msg_handler (const jerry_call_info_t *call_info_p, /**< call information */    \
                const jerry_value_t args_p[], /**< function arguments */          \
                const jerry_length_t args_cnt) /**< number of function arguments */

{
  return jerry_string_sz (my_struct.msg);
} /* get_msg_handler */

int js_example9 (int argc,char *argv[])
{
  /* Initialize engine*/ 
  jerry_init (JERRY_INIT_EMPTY);

  /* Register 'print' function from the extensions  */ 
  jerryx_register_global ("print", jerryx_handler_print);

  /* Do something with the native object*/ 
  my_struct.msg = "Hello, World!";

  /* Create an empty JS object */
  jerry_value_t object = jerry_object ();

  /* Create a JS function object and wrap into a jerry value */
  jerry_value_t func_obj = jerry_function_external (get_msg_handler);

  /* Set the native function as a property of the empty JS object */
  jerry_value_t prop_name = jerry_string_sz ("myFunc");
  jerry_value_free (jerry_object_set (object, prop_name, func_obj));
  jerry_value_free (prop_name);
  jerry_value_free (func_obj);

   /* Wrap the JS object (not empty anymore) into a jerry api value */
  jerry_value_t global_object = jerry_current_realm ();

   /* Add the JS object to the global context */
  prop_name = jerry_string_sz ("MyObject");
  jerry_value_free (jerry_object_set (global_object, prop_name, object));
  jerry_value_free (prop_name);
  jerry_value_free (object);
  jerry_value_free (global_object);

  /* Now we have a "builtin" object called MyObject with a function called myFunc()
   *
   * Equivalent JS code:
   *   var MyObject = { myFunc : function () { return "some string value"; } }
   */
  const jerry_char_t script[] = " \
    var str = MyObject.myFunc (); \
    print (str); \
  ";

   /* Evaluate script */
  jerry_value_t eval_ret = jerry_eval (script, sizeof (script) - 1, JERRY_PARSE_NO_OPTS);

  /* Free JavaScript value, returned by eval */
  jerry_value_free (eval_ret);

  /* Cleanup engine */
  jerry_cleanup ();

  return 0;
}
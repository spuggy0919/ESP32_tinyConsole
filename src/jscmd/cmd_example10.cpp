#include "jerryscript.h"
#include "jerryscript-ext.h"

/**
 * Add param to 'this.x'
 */
static jerry_value_t
add_handler (const jerry_call_info_t *call_info_p, /**< call information */
             const jerry_value_t args_p[], /**< function arguments */
             const jerry_length_t args_cnt) /**< number of function arguments */
{
  /* The the 'this_val' is the 'MyObject' from the JS code below */
  /* Note: that the argument count check is ignored for the example's case */

  /* Get 'this.x' */
  jerry_value_t prop_name = jerry_string_sz ("x");
  jerry_value_t x_val = jerry_object_get (call_info_p->this_value, prop_name);

  if (!jerry_value_is_exception (x_val))
  {
    /* Convert Jerry API values to double */
    double x = jerry_value_as_number (x_val);
    double d = jerry_value_as_number (args_p[0]);

    /* Add the parameter to 'x' */
    jerry_value_t res_val = jerry_number (x + d);

    /* Set the new value of 'this.x' */
    jerry_value_free (jerry_object_set (call_info_p->this_value, prop_name, res_val));
    jerry_value_free (res_val);
  }

  jerry_value_free (x_val);
  jerry_value_free (prop_name);

  return jerry_undefined ();
} /* add_handler */

int js_example10 (int argc,char *argv[])

{
  /* Initialize engine */
  jerry_init (JERRY_INIT_EMPTY);

  /* Register 'print' function from the extensions */
  jerryx_register_global ("print", jerryx_handler_print);

  /* Create a JS object */
  const jerry_char_t my_js_object[] = " \
    MyObject = \
    { x : 12, \
      y : 'Value of x is ', \
      foo: function () \
      { \
        return this.y + this.x; \
      } \
    } \
  ";

  jerry_value_t my_js_obj_val;

  /* Evaluate script */
  my_js_obj_val = jerry_eval (my_js_object,
                              sizeof (my_js_object) - 1,
                              JERRY_PARSE_NO_OPTS);

  /* Create a JS function object and wrap into a jerry value */
  jerry_value_t add_func_obj = jerry_function_external (add_handler);

  /* Set the native function as a property of previously created MyObject */
  jerry_value_t prop_name = jerry_string_sz ("add2x");
  jerry_value_free (jerry_object_set (my_js_obj_val, prop_name, add_func_obj));
  jerry_value_free (add_func_obj);
  jerry_value_free (prop_name);

  /* Free JavaScript value, returned by eval (my_js_object) */
  jerry_value_free (my_js_obj_val);

  const jerry_char_t script[] = " \
    var str = MyObject.foo (); \
    print (str); \
    MyObject.add2x (5); \
    print (MyObject.foo ()); \
  ";

  /* Evaluate script */
  jerry_value_t eval_ret = jerry_eval (script, sizeof (script) - 1, JERRY_PARSE_NO_OPTS);

  /* Free JavaScript value, returned by eval */
  jerry_value_free (eval_ret);

  /* Cleanup engine */
  jerry_cleanup ();

  return 0;
}
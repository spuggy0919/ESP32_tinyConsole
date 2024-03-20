#include "ESP32inc.h"

#include "cmdconfig.h"
#ifdef JERRYSCRIPT

#include "Arduino_Portenta_JerryScript.h"

// REDIRECT_STDOUT_TO(Serial);


int cmd_JerryScript_Basic(int argc,char* argv[]) {

   wsTextPrintf("Arduino Core API: %d.%d.%d\n", ARDUINO/1000, ARDUINO/100%10, ARDUINO%100);
  // wsTextPrintf("Mbed OS API: %d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
  wsTextPrintf("JerryScript API: %d.%d.%d\n", JERRY_API_MAJOR_VERSION, JERRY_API_MINOR_VERSION, JERRY_API_PATCH_VERSION);
  jerry_char_t script1[] = "print('Hello, World!中文測試\n', Math.random ());";
  long int len = 0;
  wsSerial.write((char *)script1,sizeof(script1)); 

  const jerry_char_t script[] = "print ('Hello, World!', Math.random ());";

  /* Initialize engine */
  jerry_init (JERRY_INIT_EMPTY);

  /* Set log level */
  jerry_log_set_level (JERRY_LOG_LEVEL_DEBUG);

  /* Register the print function in the global object */
  jerryx_register_global ("print", jerryx_handler_print);

  /* Run script with 'eval' */
  jerry_value_free (jerry_eval (script, sizeof (script) - 1, JERRY_PARSE_NO_OPTS)); //?? size-1

  /* Cleanup engine */
  jerry_cleanup ();
  return 0;
}
#endif //JERRYSCRIPT
// void loop() {
//   delay(1000);
// }
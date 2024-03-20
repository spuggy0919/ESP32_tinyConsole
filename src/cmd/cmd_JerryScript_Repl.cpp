#include "ESP32inc.h"
// #include <mbed.h>

#include "cmdconfig.h"
#ifdef JERRYSCRIPT
#include "Arduino_Portenta_JerryScript.h"

// REDIRECT_STDOUT_TO(Serial);


int cmd_JerryScript_Repl(int argc,char* argv[]) {


  // wsTextPrintf("\n");
  // wsTextPrintf("Arduino Core API: %d.%d.%d\n", ARDUINO/1000, ARDUINO/100%10, ARDUINO%100);
  // wsTextPrintf("Mbed OS API: %d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
  // wsTextPrintf("JerryScript API: %d.%d.%d\n", JERRY_API_MAJOR_VERSION, JERRY_API_MINOR_VERSION, JERRY_API_PATCH_VERSION);

  /* Initialize engine */
  jerry_init (JERRY_INIT_EMPTY);

  /* Set log level */
  jerry_log_set_level (JERRY_LOG_LEVEL_DEBUG);

  /* Register the print function in the global object */
  bool result = jerryxx_register_extra_api(); 
  if (result)  wsTextPrintf("jerryxx_register_extra_api\n");

  /* Register the Arduino API in the global object */
  result = jerryxx_register_arduino_api (); //not support now
  if (result)  wsTextPrintf("jerryxx_register_arduino_api\n");

  /* Read Evaluate Print Loop */
  jerryx_repl("js>");

  /* Cleanup engine */
  jerry_cleanup ();
  return 0;
}

#endif //JERRYSCRIPT
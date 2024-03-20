#include "ESP32inc.h"

#include "cmdconfig.h"
#ifdef JERRYSCRIPT

#include "Arduino_Portenta_JerryScript.h"

int  cmd_JerryScript_RunFile(int argc,char* argv[]) {

//   wsTextPrintf("Arduino Core API: %d.%d.%d\n", ARDUINO/1000, ARDUINO/100%10, ARDUINO%100);
//   // wsTextPrintf("Mbed OS API: %d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
//   wsTextPrintf("JerryScript API: %d.%d.%d\n", JERRY_API_MAJOR_VERSION, JERRY_API_MINOR_VERSION, JERRY_API_PATCH_VERSION);
    char fbuf[32];
    if (argc>1) {
        // check externsion exist or not 
        String fname = argv[1];
        String dot =".";
        if (fname.indexOf(dot) == -1) {
            fname+=".js";
            strcpy(fbuf,fname.c_str());
            argv[1]=fbuf;
        }
    }else{
        wsTextPrintf("Usage:js filename\n");
        return 1; 
    }
  /* Initialize engine */
  jerry_init(JERRY_INIT_EMPTY);

  /* Set log level */
  jerry_log_set_level(JERRY_LOG_LEVEL_DEBUG);


  /* Register the print function in the global object */
  jerryxx_register_extra_api(); 

  /* Register the Arduino API in the global object */
  jerryxx_register_arduino_api (); //not support now


  jerry_size_t result = jerryx_source_exec_script ((const char *)argv[1]);
    if (jerry_value_is_error(result)) {
       JERRYX_ERROR_MSG("exec: %d\n", jerry_value_is_error(result));
       return 2;
    }
  jerry_value_free(result);
  jerry_cleanup();
  return 0;
}

#endif
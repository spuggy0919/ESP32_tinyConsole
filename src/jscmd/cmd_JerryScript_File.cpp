#include "ESP32inc.h"

#include "cmdconfig.h"
#ifdef _LANG_JERRYSCRIPT_

#include "Arduino_Portenta_JerryScript.h"
#include "jswrap_tc.h"

String checkExtension(const char *filename){
        String fname = String(filename);
        String dot =".";
        if (fname.indexOf(dot) == -1) {
            fname+=".js";
        }
        return fname;
}
int  cmd_JerryScript_RunFile(int argc,char* argv[]) {

//   wsTextPrintf("Arduino Core API: %d.%d.%d\n", ARDUINO/1000, ARDUINO/100%10, ARDUINO%100);
// wsTextPrintf("Mbed OS API: %d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
//   wsTextPrintf("JerryScript API: %d.%d.%d\n", JERRY_API_MAJOR_VERSION, JERRY_API_MINOR_VERSION, JERRY_API_PATCH_VERSION);
    if (argc>1) {
        // check externsion exist or not 

    for(int i=1;i<argc;i++) {
            String fname_ext = checkExtension(argv[i]);
            if (!g_fsio.fexist(fname_ext.c_str())) {
            wsTextPrintf("ERROR:File not found!(%s)\n",argv[i]);
            return 1;
        }
    }
    }else{
        wsTextPrintf("Usage:js file1 [file2...]\n");
        return 1; 
    }

  /* Initialize engine */
  jerry_init(JERRY_INIT_EMPTY);
  /* Set log level */
  jerry_log_set_level(JERRY_LOG_LEVEL_DEBUG);
  /* Register the Arduino API in the global object */
  jerryxx_register_arduino_api (); //not support now
  jerryxx_register_arduino_library(); //in jswarp_tc

    for(int i=1;i<argc;i++){
        String fname_ext = checkExtension(argv[i]);
        jerry_size_t result = jerryx_source_exec_script ((const char *)fname_ext.c_str());
            if (jerry_value_is_error(result)) {
                JERRYX_ERROR_MSG("exec: %d\n", jerry_value_is_error(result));
                return 2;
            }
        jerry_value_free(result);
    }

  jerry_cleanup();
  return 0;
}

#endif //_LANG_JERRYSCRIPT_
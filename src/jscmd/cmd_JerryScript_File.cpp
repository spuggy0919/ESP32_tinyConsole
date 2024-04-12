#include "ESP32inc.h"

#include "cmdconfig.h"
#ifdef _LANG_JERRYSCRIPT_

#include "Arduino_Portenta_JerryScript.h"
#include "jswrap_tc.h"

// typedef enum
// {
//   JERRY_INIT_EMPTY = (0u), /**< empty flag set */
//   JERRY_INIT_SHOW_OPCODES = (1u << 0), /**< dump byte-code to log after parse */
//   JERRY_INIT_SHOW_REGEXP_OPCODES = (1u << 1), /**< dump regexp byte-code to log after compilation */
//   JERRY_INIT_MEM_STATS = (1u << 2), /**< dump memory statistics */
// } jerry_init_flag_t;
jerry_init_flag_t get_init(){
    String debugStr= Config_Get("initflag");
    jerry_init_flag_t initflag =  (jerry_init_flag_t)(debugStr.toInt());
    // wsTextPrintf(("initflag"+String(initflag)+"\n").c_str());
    return initflag;
}
// typedef enum
// {
//   JERRY_LOG_LEVEL_ERROR = 0u, /**< the engine will terminate after the message is printed */
//   JERRY_LOG_LEVEL_WARNING = 1u, /**< a request is aborted, but the engine continues its operation */
//   JERRY_LOG_LEVEL_DEBUG = 2u, /**< debug messages from the engine, low volume */
//   JERRY_LOG_LEVEL_TRACE = 3u /**< detailed info about engine internals, potentially high volume */
// } jerry_log_level_t;
jerry_log_level_t get_loglevel(){
    String debugStr= Config_Get("loglevel");
    jerry_log_level_t loglevel = (jerry_log_level_t)debugStr.toInt();
    // wsTextPrintf(("loglevel"+String(loglevel)+"\n").c_str());
        return loglevel;

}
int  cmd_JerryScript_RunFile(int argc,char* argv[]) {

//   wsTextPrintf("Arduino Core API: %d.%d.%d\n", ARDUINO/1000, ARDUINO/100%10, ARDUINO%100);
// wsTextPrintf("Mbed OS API: %d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
//   wsTextPrintf("JerryScript API: %d.%d.%d\n", JERRY_API_MAJOR_VERSION, JERRY_API_MINOR_VERSION, JERRY_API_PATCH_VERSION);
    if (argc>1) {
        // check externsion exist or not 

        for(int i=1;i<=1;i++) { // check argv[1] .js file only
                String fname_ext = checkExtension(argv[i]);
                if (!g_fsio.fexist(fname_ext.c_str())) {
                wsTextPrintf("ERROR:File not found!(%s)\n",argv[i]);
                return 1;
            }
        }
    }else{
        wsTextPrintf("Usage:js file1 [argv2...]\n");
        return 1; 
    }

  /* Initialize engine */
  jerry_init(get_init());
  /* Set log level */
  jerry_log_set_level(get_loglevel());
  /* Register the Arduino API in the global object */
  jerryxx_register_arduino_api (); //not support now
  jerryxx_register_arduino_library(); //in jswarp_tc

    // for(int i=1;i<=1;i++){ // run first argc only
        if (!execScriptsFile(argv[1])) return 2;
    // }
  jerryxx_free_library();
  jerry_cleanup();
  return 0;
}

#endif //_LANG_JERRYSCRIPT_
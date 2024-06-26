#include "ESP32inc.h"
// #include <mbed.h>

#include "cmdconfig.h"
#ifdef _LANG_JERRYSCRIPT_
#include "Arduino_Portenta_JerryScript.h"
#include "jswrap_tc.h"

// REDIRECT_STDOUT_TO(Serial);
#define REPL_EXIT     -1
#define REPL_NULL     0
#define REPL_CONTINUE 1
#define REPL_NORMAL   2
int repl_command_check(jerry_char_t *line_p){
  // repl command  .exit .help
  // now only exit
  if (line_p==NULL){
      jerryx_print_byte ('\n');
      return REPL_NULL;
  } 
  String cmd = String((const char*)line_p);
  if (cmd.length()==0){
      return REPL_CONTINUE;
  } 
  cmd =  cmd.substring(0, cmd.length() - 1);
  if (line_p[0]=='.'){
    int argc;
    char** argv;
      InterpreterArgvList(&cmd,&argc,&argv); // prepare argVlist
      for(int i=0;i<argc;i++)
          wsTextPrintf("[REPL]:argv[%d]=%s\n",i,argv[i]);
      if (String(".exit").startsWith(argv[0])) {
        wsTextPrintf("\n[repl]:.exit(%d)\n",REPL_EXIT);
        return REPL_EXIT; 
      }else if (String(".run").startsWith(argv[0])) {
        int ret = execScriptsFile(argv[1]);
        wsTextPrintf("\n[repl]:.run(%d)\n",ret);
      }else if (String(".parser").startsWith(argv[0])) {
        int ret = parserRunScriptsFile(argv[1]);
        wsTextPrintf("\n[repl]:.parser (%d)\n",ret);
      }else{ // try run shell cmd
        int ret = execShellCmd((const char*)&line_p[1]);
        if (ret!=0) {
            wsTextPrintf("\n[repl]:.shell error(%d)\n",ret);
        }
      }
  }
  return REPL_NORMAL;
}

void
jerryx_repl_with_cmd (const char *prompt_p,int argc,char* argv[])
{
  jerry_value_t result;
  int repl_ret;
  jerry_value_t print_result;
  jerry_value_t script;

  if (argc>1) {
      // check externsion exist or not 

      for(int i=1;i<=1;i++) { // check argv[1] .js file only
              String fname_ext = checkExtension(argv[i]);
              if (!g_fsio.fexist(fname_ext.c_str())) {
              wsTextPrintf("ERROR:File not found!(%s)\n",argv[i]);
              return;
          }
      }
      if (parserRunScriptsFile(argv[1])) {
          wsTextPrintf("ParserRun Load(%s)\n",argv[1]);
      }
  }

  while (true)
  {
    jerryx_print_string (prompt_p);

    jerry_size_t length;
    jerry_char_t *line_p = jerry_port_line_read (&length);

    repl_ret = repl_command_check(line_p);
    if (repl_ret <= REPL_NULL) {
      return;  // REPL_EXIT < REPL_NULL
    }
    if (repl_ret == REPL_CONTINUE) {continue;}

    if (!jerry_validate_string (line_p, length, JERRY_ENCODING_UTF8))
    {
      jerry_port_line_free (line_p);
      result = jerry_throw_sz (JERRY_ERROR_SYNTAX, "Input is not a valid UTF-8 string");
      goto exception;
    }

    result = jerry_parse (line_p, length, NULL);
    jerry_port_line_free (line_p);

    if (jerry_value_is_exception (result))
    {
      goto exception;
    }

    script = result;
    result = jerry_run (script);
    jerry_value_free (script);

    if (jerry_value_is_exception (result))
    {
      goto exception;
    }

    print_result = jerryx_print_value (result);
    jerry_value_free (result);
    result = print_result;

    if (jerry_value_is_exception (result))
    {
      goto exception;
    }

    jerryx_print_byte ('\n');

    jerry_value_free (result);
    result = jerry_run_jobs ();

    if (jerry_value_is_exception (result))
    {
      goto exception;
    }

    jerry_value_free (result);
    continue;

  exception:
    jerryx_print_unhandled_exception (result);
  }
} /* jerryx_repl */



int cmd_JerryScript_Repl(int argc,char* argv[]) {


  // wsTextPrintf("\n");
  wsTextPrintf("Arduino Core API: %d.%d.%d\n", ARDUINO/1000, ARDUINO/100%10, ARDUINO%100);
  // wsTextPrintf("Mbed OS API: %d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
  wsTextPrintf("JerryScript API: %d.%d.%d\n", JERRY_API_MAJOR_VERSION, JERRY_API_MINOR_VERSION, JERRY_API_PATCH_VERSION);

  /* Initialize engine */
  jerry_init (JERRY_INIT_EMPTY);

  /* Set log level */
  jerry_log_set_level (JERRY_LOG_LEVEL_DEBUG);

  /* Register the print function in the global object */
  bool result = jerryxx_register_extra_api(); 
  // if (result)  wsTextPrintf("jerryxx_register_extra_api\n");

  /* Register the Arduino API in the global object */
  result = jerryxx_register_arduino_api (); //not support now
  // if (result)  wsTextPrintf("jerryxx_register_arduino_api\n");
  jerryxx_register_arduino_library(); //in jswarp_tc

  /* Read Evaluate Print Loop */
  jerryx_repl_with_cmd("js>",argc,argv);

  /* Cleanup tinyconsole resource */
  jerryxx_free_library();
  /* Cleanup engine */
  jerry_cleanup ();
  return 0;
}

#endif //_LANG_JERRYSCRIPT_
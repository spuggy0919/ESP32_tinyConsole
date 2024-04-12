/*
 * This file is part of ESP32_TinyConsole.
 *
 *  Copyright (c) 2024 spuggy0919
 *
 * ESP32_TinyConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESP32_TinyConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ESP32_TinyConsole.  If not, see 
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */

#include "jswrap_tc.h"
#include "ESP32inc.h"

// String reportfs(fs::FS &fs);
JERRYXX_DECLARE_FUNCTION(fs_info){ //1
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
       //5
  return jerry_string_sz(reportfs(LittleFS).c_str());
} /*js_fs_info*/ //6

// String  getFullPath_File(const char *pname, const char *fname);
// String currentDir(); //pwd
// String changeDir(fs::FS &fs, const char *path);
JERRYXX_DECLARE_FUNCTION(fs_cd){ //1
  JERRYX_UNUSED(call_info_p);
  char      buf[64];
  uint32_t  len=64;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  // JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4  
  if (args_cnt == 0) {   //5
      return jerry_string_sz(currentDir().c_str());
  }
  if (args_cnt == 1) {   //5
      return jerry_string_sz(changeDir(LittleFS,buf).c_str());
  }
  return jerry_undefined();
} /*js_fs_cd*/ //6

// String getfullpathFileOrDir(fs::FS &fs, const char *pname);
// String listDir(fs::FS &fs, const char * dirname, uint8_t levels);
JERRYXX_DECLARE_FUNCTION(fs_ls){ //1
  JERRYX_UNUSED(call_info_p);
  char      buf[64];
  uint32_t  len=64;
  uint32_t  level;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
          jerryx_arg_uint32(&level, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  if (args_cnt == 0) {   //5
      String dirlist = listDir(LittleFS, "" ,1 );
      return jerry_string_sz(dirlist.c_str());
  }
  if (args_cnt == 1) {   //5
      String dirlist = listDir(LittleFS, buf,1 );
      return jerry_string_sz(dirlist.c_str());
  }
  if (args_cnt == 2) {   //5
      String dirlist = listDir(LittleFS, buf,(uint8_t ) level);
      return jerry_string_sz(dirlist.c_str());
  }
  return jerry_undefined();
} /*js_fs_ls*/ //6
// void createDir(fs::FS &fs, const char * path);
JERRYXX_DECLARE_FUNCTION(fs_mkdir){ //1
  JERRYX_UNUSED(call_info_p);
  char      buf[64]; //2
  uint32_t  len=64;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

    createDir(LittleFS, buf);
  return jerry_undefined();
} /*js_fs_mkdir*/ //6

// void removeDir(fs::FS &fs, const char * path);
JERRYXX_DECLARE_FUNCTION(fs_rmdir){ //1
  JERRYX_UNUSED(call_info_p);
  char      buf[64]; //2
  uint32_t  len=64;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(buf, sizeof(buf), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

    removeDir(LittleFS, buf);
  return jerry_undefined();
} /*js_fs_rmdir*/ //6

// String readFile(fs::FS &fs, const char * path);
JERRYXX_DECLARE_FUNCTION(fs_readfile){ //1
  JERRYX_UNUSED(call_info_p);
  char      fname[64]; //2
  uint32_t  len=64;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(fname, sizeof(fname), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

      String content = readFile(LittleFS, fname);
      return jerry_string_sz(content.c_str());
} /*js_fs_readfile*/ //6


// int writeFile(fs::FS &fs, const char * path, const char * message);
JERRYXX_DECLARE_FUNCTION(fs_writefile){ //1
  JERRYX_UNUSED(call_info_p);
  if (!jerry_value_is_string(args_p[0]) || !jerry_value_is_string(args_p[1])) {
    return jerry_number(0); // parameters type wrong
  }

 // Get the fname, data
    jerry_value_t fnamebuf = args_p[0];
    // Get the size of the string
    jerry_length_t fnamesize = jerry_string_size(fnamebuf,JERRY_ENCODING_UTF8);
    // Allocate a buffer for the UTF-8 string (optional)
    char* fname =(char*) malloc(fnamesize + 1); // +1 for null terminator
    jerry_string_to_buffer(fnamebuf,JERRY_ENCODING_UTF8, (jerry_char_t*)fname, fnamesize+1);
    fname[fnamesize]=0;
    jerry_value_t databuf = args_p[1];
    // Get the size of the string
    jerry_length_t databufsize = jerry_string_size(databuf,JERRY_ENCODING_UTF8);
    // Allocate a buffer for the UTF-8 string (optional)
    char* data = (char*) malloc(databufsize + 1); // +1 for null terminator
    // Convert the JerryScript string to UTF-8 and store it in the buffer
    jerry_string_to_buffer(databuf,JERRY_ENCODING_UTF8, (jerry_char_t*)data, databufsize+1);
    // Process the UTF-8 string (e.g., print it)
    data[databufsize] = 0;
    Serial.printf("[jsw_fs]:writefile string: %s(%d), %s(%d)\n", fname,fnamesize,data,databufsize);
    int len = writeFile(LittleFS, fname, data);
    // Clean up if needed

    free(fname);
    free(data);


    // Return undefined (or any appropriate return value)
      return jerry_number(len);
} /*js_fs_writefile*/ //6

// void appendFile(fs::FS &fs, const char * path, const char * message);
JERRYXX_DECLARE_FUNCTION(fs_appendfile){ //1
  JERRYX_UNUSED(call_info_p);
  if (!jerry_value_is_string(args_p[0]) || !jerry_value_is_string(args_p[1])) {
    return jerry_number(0); // parameters type wrong
  }

 // Get the fname, data
    jerry_value_t fnamebuf = args_p[0];
    // Get the size of the string
    jerry_length_t fnamesize = jerry_string_size(fnamebuf,JERRY_ENCODING_UTF8);
    // Allocate a buffer for the UTF-8 string (optional)
    char* fname =(char*) malloc(fnamesize + 1); // +1 for null terminator
    jerry_string_to_buffer(fnamebuf,JERRY_ENCODING_UTF8, (jerry_char_t*)fname, fnamesize+1);
    fname[fnamesize]=0;

    jerry_value_t databuf = args_p[1];
    // Get the size of the string
    jerry_length_t databufsize = jerry_string_size(databuf,JERRY_ENCODING_UTF8);
    // Allocate a buffer for the UTF-8 string (optional)
    char* data = (char*) malloc(databufsize + 1); // +1 for null terminator
    // Convert the JerryScript string to UTF-8 and store it in the buffer
    jerry_string_to_buffer(databuf,JERRY_ENCODING_UTF8, (jerry_char_t*)data, databufsize+1);
    data[databufsize] = 0;

    // Process the UTF-8 string (e.g., print it)
    Serial.printf("[jsw_fs]:appendfile string: %s(%d), %s(%d)\n", fname,fnamesize,data,databufsize);

    appendFile(LittleFS, fname, data);
    // Clean up if needed

    free(fname);
    free(data);


    // Return undefined (or any appropriate return value)
   return jerry_undefined();
} /*js_fs_appendfile*/ //6

// void renameFile(fs::FS &fs, const char * path1, const char * path2);
JERRYXX_DECLARE_FUNCTION(fs_renamefile){ //1
  JERRYX_UNUSED(call_info_p);
  char      fname[64]; //2
  char      fname1[64]; //2
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(fname, sizeof(fname), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
          jerryx_arg_string(fname1, sizeof(fname1), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
    return rv;
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 2, "Wrong arguments count");  //4

     renameFile(LittleFS, fname,fname1);
   return jerry_undefined();
} /*js_fs_renamefile*/ //6

// void deleteFile(fs::FS &fs, const char * path);
JERRYXX_DECLARE_FUNCTION(fs_deletefile){ //1
  JERRYX_UNUSED(call_info_p);
  char      fname[64]; //2
  uint32_t  len=64;
  const jerryx_arg_t mapping[] = //3
      {
          jerryx_arg_string(fname, sizeof(fname), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),
      };
  const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));
  if (jerry_value_is_exception(rv)){
   return jerry_undefined();
  }
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 1, "Wrong arguments count");  //4

      deleteFile(LittleFS, fname);
   return jerry_undefined();
} /*js_fs_deletefile*/ //6

JERRYXX_DECLARE_FUNCTION(fs_classname){
  JERRYX_UNUSED(call_info_p);
  JERRYX_UNUSED(args_p);
  JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");
  return jerry_string_sz("fslib");
} /*js_fs_classname*/



// class object wrapper
bool js_tcfilelib_classobj_wraper(){
  bool ret = false;
          // Register initialization function
  jerryx_property_entry methods[] ={
    JERRYX_PROPERTY_STRING_SZ ("id", "fs"),
    JERRYX_PROPERTY_FUNCTION ("df", js_fs_info),
    JERRYX_PROPERTY_FUNCTION ("cd", js_fs_cd),
    JERRYX_PROPERTY_FUNCTION ("ls", js_fs_ls),
    JERRYX_PROPERTY_FUNCTION ("mkdir", js_fs_mkdir),
    JERRYX_PROPERTY_FUNCTION ("rmdir", js_fs_rmdir),
    JERRYX_PROPERTY_FUNCTION ("readFile", js_fs_readfile),
    JERRYX_PROPERTY_FUNCTION ("writeFile", js_fs_writefile),
    JERRYX_PROPERTY_FUNCTION ("appendFile", js_fs_appendfile),
    JERRYX_PROPERTY_FUNCTION ("deleteFile", js_fs_deletefile),
    JERRYX_PROPERTY_LIST_END(),
  };


  jerry_value_t object = jerry_object();
  jerryx_register_result reg = jerryx_set_properties (object, methods);

  if (jerry_value_is_exception (reg.result))
  {
    wsTextPrintf ("Only registered %d properties\r\n", reg.registered);
    /* clean up not registered property values */
    jerryx_release_property_entry (methods, reg);
    jerry_value_free (reg.result);
    return false;
    /* clean up the error */
  }
  /* Set the native function as a property of the empty JS object */

  // wrap to dht 
  jerry_value_t globalObject = jerry_current_realm ();
  jerry_value_t prop_name = jerry_string_sz((const char*)"File");
  jerry_value_free (jerry_object_set (globalObject, prop_name, object));


  jerry_value_free (object);
  jerry_value_free (prop_name);
  jerry_value_free (globalObject);

  return true;
}

jerry_value_t register_fs_module(){
          // Register initialization function
  jerryx_property_entry methods[] ={
    JERRYX_PROPERTY_FUNCTION ("name", js_fs_classname),
    JERRYX_PROPERTY_FUNCTION ("df", js_fs_info),
    JERRYX_PROPERTY_FUNCTION ("cd", js_fs_cd),
    JERRYX_PROPERTY_FUNCTION ("ls", js_fs_ls),
    JERRYX_PROPERTY_FUNCTION ("mkdir", js_fs_mkdir),
    JERRYX_PROPERTY_FUNCTION ("rmdir", js_fs_rmdir),
    JERRYX_PROPERTY_FUNCTION ("readFile", js_fs_readfile),
    JERRYX_PROPERTY_FUNCTION ("writeFile", js_fs_writefile),
    JERRYX_PROPERTY_FUNCTION ("appendFile", js_fs_appendfile),
    JERRYX_PROPERTY_FUNCTION ("deleteFile", js_fs_deletefile),
    JERRYX_PROPERTY_LIST_END(),
  };


  jerry_value_t object = jerry_object();
  jerryx_register_result reg = jerryx_set_properties (object, methods);

  if (jerry_value_is_exception (reg.result))
  {
    wsTextPrintf ("Only registered %d properties\r\n", reg.registered);
    /* clean up not registered property values */
    jerryx_release_property_entry (methods, reg);
    jerry_value_free (reg.result);
    return false;
    /* clean up the error */
  }
  /* Set the native function as a property of the empty JS object */
  // wrap to fs module

  jerry_value_t exports_object = jerry_object();
  jerry_value_t module_name = jerry_string_sz((const char*)"fs");
  jerry_value_free (jerry_object_set (exports_object, module_name, object));


  jerry_value_free (object);
  jerry_value_free (module_name);
  jerry_value_free (exports_object);

  return exports_object;
}

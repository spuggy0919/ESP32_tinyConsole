
/*
 * This file is part of ESP32_TinyConsole.
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
#include "command.h"

extern const int  CMDNUMBERS ;
extern const int  CMDTBLSIZE ;

/* 0x3f53fc00 last 1K block*/
// COMMAND_TABLE commandTable[] __attribute__((section(".tinyconsole_cmdtbl_section"))) = {
COMMAND_TABLE commandTable[] = {
  (const char*)0xaa551234, (MAINPTR)CMDTBLSIZE,(const char*)CMDNUMBERS,
#ifdef TINYBASIC
  "tb",     cmd_tinybasic,  "file.bas\t// tinyBasic run file",
  "tinybasic",  cmd_tinybasic,  "\t// tinyBasic interactive",
#endif
#ifdef JERRYSCRIPT
  "jb",   cmd_JerryScript_Basic,   "\t\t// JerryScript Basic ",
  "js",   cmd_JerryScript_RunFile,   "\t\t// JerryScript runfile",
  "jerryscript",   cmd_JerryScript_Repl,   "\t// JerryScript Repl",
#endif 
  "pwd",    cmd_pwd,    "\t\t// current path",
  "cd",     cmd_cd,     "\t\t// change directory",
  "fp",     cmd_fp,     "path \t// full path testing",
  // "ls0",    cmd_ls0,    "      // dummy list",
  "df",     cmd_df,     "\t\t// disk infomation",
  "ls",     cmd_ls,     "path  \t// list directory",
  "mkdir",  cmd_mkdir,  "path  \t// make directory ",
  "rm",     cmd_rm,     "file  \t// remove file",
  "rmdir",  cmd_rmdir,  "path  \t// remove directory",
  "cat",    cmd_cat,    "file  \t// diplay file content",
  "echo",   cmd_echo,   "msg   \t// echo message",
  "write",  cmd_write,  "file msg \t// write message" ,
  "append", cmd_append, "file msg // append message",
  "cp",     cmd_cp,     "file1 file2 \t// copy file1 to file2",
  "mv",     cmd_mv,     "file1 file2 \t// rename file1 to file2",
#ifdef CMD_TIME
  "time",   cmd_time,   "\t\t// display currnet time",
#endif
  "pio",    cmd_pio,    "0~255 \t// set LED pin output pwm value",
  "dl",     cmd_download,"file  \t// download file to PC",
  "cls",    cmd_cls,    "\t\t// clear screen",
 "avtest",  cmd_avtest, "[0|1|2]\t// video graphic testing",
 "export",  cmd_export, "\t\t// export ssid ABC \n \t\t// export password 12345678 \n\t\t// export\n\t\t// export all wifi config or set config", 
  "loop",   cmd_test,  "\t\t// loopback test and arglist parser Test",
#ifdef CMD_DHT
  "dht",    cmd_dht,    "\t\t// dht example",
#endif
#ifdef CMD_MQTT
  "mqtt",   cmd_mqtt,   "\t\t// mqtt example",
#endif
#ifdef CMD_MESH
  "mesh",   cmd_mesh,   "\t\t// mesh experiment", //not ready
#endif
  "exec",   cmd_exec,   "\t\t// exec \"cmd argv[]\" for task experiment",
  "kill",   cmd_kill,   "\t\t// kill process, for task experiment ",
  "ps",     cmd_ps,     "\t\t// process status, for task experiment",
  "reboot", cmd_reboot,  "\t\t// reboot",
  "help",   cmd_help,   "\t\t// help ",
  "h",      cmd_help,   "\t\t// help",
  "?",      cmd_help,   "\t\t// help",
  "hello",   cmd_hello,   "\t\t// hello world  example",
 //  below internal teing commands and help qill not show 
 /*2. plug in your cmd into shell interpreter cmd table */
  "ping",   cmd_test,   "\t\t// ping test, int in monitor",

  "status", cmd_status,  "status",
"test",   cmd_test,    "\t// dummy command",
"task",    cmd_task,  "  // tasktest testing...",
 "dump",    cmd_dumpHex,   "0xaddr len //  dump Physical memory testing...",
 "call",    cmd_call,     "address argv ...// call cmd function  testing...",
 "tdelay",  cmd_taskdelay,     "delaynum...// for wssockettask idle time...",
 "option",  cmd_option,     "arglist // for '-' option check...",
 "millis",  cmd_millis,     " // for '-' option check...",
 "mdelay",  cmd_millisdelay,     "delaycnt // for '-' option check...",
 "video",  cmd_video,       "video 0  // video graphics functions",
 "exehello",  cmd_exehello,       "exehello jmp hello", //experiment

  "-",      cmd_prompt,   "",    // End of Command Table
   "",      cmd_prompt,   ""
};

const int  CMDTBLSIZE = (sizeof(commandTable));
const int  CMDNUMBERS = (CMDTBLSIZE/sizeof(COMMAND_TABLE));

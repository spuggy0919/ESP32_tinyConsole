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
#ifndef __COMMAND_H__
#define __COMMAND_H__
#include <Arduino.h> // String used


#define TINYBASIC
#ifdef TINYBASIC
int cmd_tinybasic(int argc,char * argv[]);
#endif

int cmd_help(int argc,char * argv[]);
int cmd_echo(int argc,char * argv[]);
int cmd_pwd(int argc,char * argv[]);
int cmd_time(int argc,char * argv[]);
int cmd_cd(int argc,char * argv[]);
int cmd_fp(int argc,char * argv[]);
int cmd_append(int argc,char * argv[]);
int cmd_write(int argc,char * argv[]);
int cmd_ls(int argc,char * argv[]);
int cmd_ls0(int argc,char * argv[]);
int cmd_df(int argc,char * argv[]);
int cmd_mkdir(int argc,char * argv[]);
int cmd_rmdir(int argc,char * argv[]);
int cmd_cat(int argc,char * argv[]);
int cmd_rm(int argc,char * argv[]);
int cmd_download(int argc,char * argv[]);
int cmd_cp(int argc,char * argv[]);
int cmd_mv(int argc,char * argv[]);
int cmd_status(int argc,char * argv[]);
int cmd_ping(int argc,char * argv[]);
int cmd_prompt(int argc,char * argv[]);
int cmd_call(int argc,char * argv[]);
int cmd_pio(int argc, char *argv[]);
int cmd_cls(int argc,char * argv[]);
int cmd_taskdelay(int argc, char *argv[]);
int cmd_millisdelay(int argc, char *argv[]);
int cmd_millis(int argc, char *argv[]);
int cmd_option(int argc, char *argv[]);
int cmd_video(int argc, char *argv[]);
int cmd_avtest(int argc, char *argv[]);
int cmd_export(int argc, char *argv[]);
int cmd_reboot(int argc,char * argv[]);
// main_cmd list
int cmd_mesh(int argc,char *argv[]);
int cmd_test(int argc, char *argv[]);
int cmd_dumpHex(int argc,char * argv[]);
int cmd_task(int argc, char *argv[]);
int cmd_exec(int argc, char *argv[]);
int cmd_kill(int argc, char *argv[]);
int cmd_ps(int argc, char *argv[]);/*How to pluginto your command */
int cmd_exehello(int argc,char * argv[]); // experiment
int test_main(int argc,char * argv[]);
int cmd_dht(int argc,char *argv[]);
int cmd_mqtt(int argc,char *argv[]);
/*1. cmd_example declare your command prototype here */
int cmd_hello(int argc,char *argv[]);

typedef int (*MAINPTR)(int argc,char * argv[]);

typedef struct COMMAND_TABLE_ITEM{
   const char *   funName;
   MAINPTR  cmdPtr;
   const char *   description;
} COMMAND_TABLE; 


extern COMMAND_TABLE commandTable[];
extern const int  CMDNUMBERS;
#endif  //__INTEPRETER_H__
# Here is C source example, which can integrate into my interpreter
## 1. write your program with normal c entry, see cmd_hello.c
```
#include "ESP32inc.h"
/* include your library if need and add to ini library dep*/
#include "plotLabel.h"

int state = 0;
void cmd_hello_setup(){

}
bool cmd_hello_loop(){
    if (wsSerial.escape()) return false; //escape key, crtl-c, q [enter]

    wsTextPrintf("Hello World\n"); // websocket print to console
    delay(1000);
    return true;
}
```
## 2. add this cmd into COMMAND_TABLE commandTable[] in /AAA/command.cpp and declare your prototype in command.h
### 2.1 declare your function prototype in command.h
```
/*1. cmd_hello declares in command.h prototype here */
int cmd_hello(int argc,char *argv[]);
```
### 2.2 plug into your cmd, and assign cmd name, description. Notics:comand name should be loweracse.
```
  "?",      cmd_help,   "\t\t// help",
 //  below internal teing commands
  "hello",   cmd_hello,   "\t\t// dht example",   /* Here is */
```  
### 3. background execution
run a background task, only one task allow, 
keyboard/console io will be ignored, if possible, use wsMonitorPrintf
and use stack memory, to avoid task is killed then memory is not free.

``` 
%exec exmaple[enter] // run in background
%ps // report task status
%kill [enter] // kill porcess

```  
### 4. cmdconfig.h 
To enable/disable you command defined in commandTable.

Done, compiler and burn into flash, now is static, maybe dynamic plugin in future.
# Here is C source exmaple, which can integrate into my interpreter
## 1. write your program with normal c entry, see cmd_hello.c
```
#include "ESP32inc.h"
/* include your librar and add to ini library depe*/
#include "plotLabel.h"

int state = 0;
void cmd_hello_setup(){

}
bool cmd_hello_loop(){
    if (KEYHIT()!=0) {
        c=KEYGET();
        if (c==KEY_CRTL_C) return false;
    }
    return true;
}
```
## 2. add this cmd into COMMAND_TABLE commandTable[] in command.c and declare your prototype in command.h
### 2.1 declare your function prototype in command.c
```
/*1. cmd_hello declares in command.h prototype here */
int cmd_hello(int argc,char *argv[]);
```
2.2 plug into your cmd, and assign cmd name.
```
  "?",      cmd_help,   "\t\t// help",
 //  below internal teing commands
  "hello",   cmd_hello,   "\t\t// dht example",   /* Here is */
```  
### 3. Done, now is static, maybe dynamic plugin in future.
#### Portting driver layer of tinyConsole
tinybasic port bases on tinyconsole driver layer 

1.  **wsSerial.cpp** serial driver communicate between basic and websocket client, wsSerial also respones for client **Keyboard** input and **console out**.
2. **Itouch.cpp** **touch** event queuing driver , streams come from websocket client the console cmd app or basic filter the event queuq, to get coordinate.
3. display **graphics** functions are in one include **#include "audiovideo.h"**
4. **File system** is littlefs, the tinybasic uses old littlefsfun.h library, my new interface is **fileio.h** class object, global name is **g_fsio**.
   	
#### TinyConsole Library functions
1. **ESPMCU** defines ESP reset and wdt clear.
2. **GPIO** for button and Led pwm setting.
3. **LITTLEFSFUN** a LITTLEFS library, defined in littlefsfun.h.In future, fileio.cpp will maintain the file system api. 
4. **timer**, esp32 rtc
5. **Tinybasic** ver 1.4a
6. **Wifi**, config.json is checked at power on, then decides AP, STA auto , or STA mode
7. **Webserver** , HTTPD Webserver and Websocket functions.
   
#### TinyBasic v2 port pending:
It needs more time to complete, I decide to pending the Basic 2 port temporarily. 

#### Port TinyBasic v2 current status
**runtime.cpp runtime.h**
1. port runtime dependency, there are serial, vga display file system is **runtime-HTTPWS-port **
2. **runtime.cpp** modifications
 2.1 ioinit modified  add ITOUCH with device 3, and vgabegin enable
  #define ITOUCH 3 /* locate in **runtime.h**
 2.2 checkch ITOUCH
 2.3 availch ITOUCH
 2.4 ins ITOUCH
 For display VGA
 2.5  undefined below function, we will replace
      void vgabegin(){}
int vgastat(uint8_t c) {return 0; }
void vgawrite(char c){}

      netbegin... are all disable
    2.6 disable the fs functions , here will replace by below item.

### runtime driver interface
 The file runtime-HTTPWS-port contain tinybasic runtime driver which inteface with draiver layers of tinyConsole with ESP32
 
 **runtime-HTTPWS-port.h**

1. Use LITTLEFS 
1. HTTP websocket serial 
2. HTTP websocket graphics command and COLOR
3. HTTP active all wifi network so disable the network connecting check
copy from my original  port tinybasic 1.4a
put last line of runtime.cpp

### how to use
copy into and replace the tinyBasic directory of my tinyconsole project, then build tinyconsole.
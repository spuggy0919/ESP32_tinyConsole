#### Portting driver layer of tinyConsole
tinybasic port on tinyconsole is 
base on my driver layers for tinyconsole
1.  **wsSerial.cpp** serial driver communicate with basic and websocket
    extern WebSocketSerial wsSerial. it also respone for 
    **Keybard** input and **console out** text out, use 
2. **Itouch.cpp** **touch** event queue driver , stream come from websocket client side
3. display **graphics** functions in one include
   **include "audiovideo.h"**
4. **File system** is littlefs, the tinybasic use old littlefsfun.h library, my new interface is 
    **fileio.h** class object, global name is **g_fsio**	
#### TinyConsole Library functions
1. **ESPMCU** define ESP reset wdt check.
2. **GPIO** for button and Led pwm setting.
3. **LITTLEFSFUN** a LITTLEFS library, define in littlefsfun.h, this will be deprecaded, fileio.cpp will maintain the file system api.
4. **timer**, esp32 rtc
5. **Tinybasic** ver 1.4a
6. **Wifi**, config.json is checked at power on, then decide AP, STA auto , STA statis
7. **Webserver** , HTTPD Webserver and Websocket functions.
   
#### TinyBasic v2 port pending:
This port is buggy, I need more time to reach goal, i decide to pending the Basic 2 port. Then will transfer to JerryScript porting first.

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


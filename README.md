# [中文](./讀我.md)
# ESP32 tinyConsole with TinyBasic
This firmware with web browser is a console, can upload, download, run TinyBasic on ESP32 board online. User can learn TinyBasic Language.
ESP32 Web Server (tinyConsole) use me-no-dev/ESPAsyncWebServer, bases on WebSocket to implement bidirection serial port.

## Functions
  
1. browser LittleFS File System of ESP32.
2. upload or download file to File System.
3. You can use Phone APP, Pad App to edit program, then upload for running on ESP32 Board.
4. TinyBasic Program can be upload to LittleFS, and run by TinyBasic Interpreter.
5. TinyBasic with interactive mode, it can write program.
6. simple and tiny shell commands, ls, cat, cd, cp, append, rm, mkdir, help. But it is simple and no option operation, no wildcard filter.

TinyBasic is ported from slviajero/tinybasic.
Currently, tinyConsole only supports DigitIO, Timer, FileIO, and WebSocket Serial IO.

# TinyBasic LICENSE:

 [https://github.com/slviajero/tinybasic](https://github.com/slviajero/tinybasic) for copyright/left.

(GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007)

# Platform.IO IDE
 ESP32 is supported
 *Platform.io* will check dependecy and download dependency libraries automatically.
 Tool bar at bottom , click Switch  env icon to select Project and env
### For ESP32
```
Default(esp32_TinyConsole) folder*
```
### For ESP8266 (maybe, but no timer function, it is not verified)<br>


# Modified *WifiSetting.h*。SSID, PASSWORD, 

### suggest STAAUTO Mode.

```
#define WIFISSID    ("SSIDNAME")        // SSID
#define PASSWORD    ("12345678")                   // PASSWORD
#define LOCAL_IP    ("192.168.4.210")     // for static sta
#define GATEWAY     ("192.168.4.1")       // for static sta or AP
#define NMASK       ("255.255.255.0")      // for static sta or AP
#define GATEWAY     ("192.168.4.1")       // for static sta or AP
#define WIFIAPNAME  ("ESPCONSOLE-")       // for AP will appen four mac chars

/* Select your WIFI MODE*/
#define  WIFISTAAUTO
#undef  WIFISTASTATIC
#undef  WIFIAPMODE
```

# Build and Upload, need to push EN button.

![pio Toolbar](png/toolbar.png)

## *Notics* upload binary file or data to file system, you must close serial monitor to let seril driver be free.

  In toolbar, (-> icon) can build and upload.
  for clean build, you can click clear icon, or delete *.pio* sub folder in explorer Window at first.
  After builded, the binary file can be upload from [>] terminal icon. Use below command.
  The path should be same with project path.
```
ESP32_TinyConsole % pio run -t upload [enter]
```
# upload file system, and need to push EN button.
 It will upload data folder to partition SPIFFS, it is OK.
 Click the pio tool bar terminal icon, then cli terminal will display, make sure path with your data then run command in terminal.
```
ESP32_TinyConsole % pio run -t uploadfs [enter]
```

# Running, click serial Monitor (plug icon) and reset button of ESP32 will display below Messages
```
Web Control
PWM init...done!
WiFiSTA() AutoMode 
WiFiSTAStatic(ssid SSID,pwd PASSWORD,ip, gw ) 
if ip and gateway is null then Auto Sta mode
Config as auto IP
Connecting to WiFi...
..192.168.1.118
stdioRedirector
WebServerGPIOControl Starting...
```
#  Login Console, Copyright Notices.
![login](png/login.png)

#  Keyin help enter, these command are simple, case sensitive, and still awkward.
```
%help [enter]
```
![Help](png/help.png)
####  upload, click Select file button then dialog show up, file selected, click Upload button.
####  dl file for download file to client。
```
%dl hello.bas [enter]
```
####  Run TinyBasic interpreter, use "TinyBasic" or "tb", then '>' in Tinybasic Interactive mode.
```
%TinyBasic hello.bas [enter]
```
```
%tb hello.bas [enter]
Hello World
>list
10 REM "The inevitable Hello World"
100 PRINT "Hello World"
> save "h.bas" 
> new
> load "blinkwod.bas"   // blink basic example for ESP32 
> run                   // press '#' for break running loop
>Crtl-C                 // press 'Crtl-C ' back to tinyConsole prompt '%' shell mode
%

```
ESP32 SSD1306 Pin 21 SDA, Pin 22 SDL, Vcc 3.3V, GND, run
```
%tb SSD1306.bas  // or in TinyBasic use Load "SSD1306.bas"
>
```
Blink example
```
%tb blinkwod.bas
```
# Demo
[Demo](https://youtu.be/75JKm-M19pE)

### TinyConsole built in TinyBasic, I have not run all examples yet!

Remember to modify Pin definition for ESP32, some examples are for Arduino.
for example LED pin 2 , EN button pin 0, and ESP32 3.3V.

[TinyBasic src](https://github.com/slviajero/tinybasic)

[TinyBasic Examples](https://github.com/slviajero/tinybasic/tree/main/examples)

# TinyBasic manual
[TinyBasic Manual](https://github.com/slviajero/tinybasic/blob/main/MANUAL.md)

# Good Luck！ 

[Back to top](#)
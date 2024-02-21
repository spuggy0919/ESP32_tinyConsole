#ifndef __LITTLEFSFUN__H__
#define __LITTLEFSFUN__H__
#include <Arduino.h>
#include "debuglog.h"

/* fail to both def , the dependency isnot same as comment this block! */

// #ifdef ESP32

#include <FS.h>
#include <LittleFS.h>

#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
 #include <time.h>
#endif
#define FORMAT_LITTLEFS_IF_FAILED true
/* comment for dependency conflict with ESP32, if ESP8266 turn on below */

#if ESP8266
// #include <FS.h>
// #include <LittleFS.h>
// #define SPIFFS LittleFS
// #define LITTLEFS LittleFS
#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_READWRITE "rw"
#define FILE_APPEND "a"
// #define FORMAT_LITTLEFS_IF_FAILED 
#endif
#define SPIFFS LittleFS
bool initLittleFS();
String reportfs(fs::FS &fs);
String  getFullPath_File(const char *pname, const char *fname);
String currentDir();
String getfullpathFileOrDir(fs::FS &fs, const char *pname);

String changeDir(fs::FS &fs, const char *path);
String listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
String readFile(fs::FS &fs, const char * path);
int writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);


// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
void writeFile2(fs::FS &fs, const char * path, const char * message);
void deleteFile2(fs::FS &fs, const char * path);

void testFileIO(fs::FS &fs, const char * path);


#define DIRTYPE  1
#define FILETYPE 2
#define NOTEXIST 3
#endif
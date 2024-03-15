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
#ifndef __FILEIO_H__
#define __FILEIO_H__
#include "littlefsfun.h"

#include <stdio.h>
// # LITTLEFS use File as FILE this Class convert back
typedef union 
{
     FILE *stdf;
     void *fsf;
} FILEDESC;


// namespace fs
// { // portstd here
    class pstd: public fs::LittleFSFS 
    { // base on littlefs and can be ported with SD
    public:
        // pstd();
        pstd(fs::LittleFSFS& littlefs); // wrap parents LITTLEFS
        ~pstd();
        void driver(const char *drv);
        bool init();
        int fclose(FILE *stream);
        FILE *fopen(const char *filename, const char *mode);
        size_t fread(const char *ptr, size_t size, size_t nmemb, FILE *stream);
        size_t fwrite(const char *ptr, size_t size, size_t nobj, FILE *stream);
        int fseek(FILE *stream, long int offset, int whence);
        long int ftell(FILE *stream);
        int remove(const char *filename);
        int rename(const char *old_filename, const char *new_filename);
        bool mkdir(const char *path);
        bool rmdir(const char *path);
        // int fprintf(File *stream, const char *format, ...)
        // int printf(const char *format, ...)
        // int sprintf(char *str, const char *format, ...)
        // int vfprintf(File *stream, const char *format, va_list arg)
    private:
      FILE* setFile(File file);
      File getFile(FILE* i);
      File closeFile(FILE* i);
      bool initReady=false;
      LittleFSFS& littlefsRef;
      static constexpr int ARRAY_SIZE=10;
      File fileArray[ARRAY_SIZE];
      bool isEmpty[ARRAY_SIZE];
    };

// // below define to convert type
// #define STD_FILE() (fileobj.stdf)
// #define FS_FILE() ((File*)fileobj.fsf)
// #define STD_ASSIGN(x) fileobj.stdf=(x)
// #define FS_ASSIGN(x)  fileobj.fsf=(&x)

extern pstd g_fsio;
// } // namespace name
#endif //__FILEIO_H__

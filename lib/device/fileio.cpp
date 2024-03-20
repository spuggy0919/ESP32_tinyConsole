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
/*
 * @file fileio.cpp
 * @brief  wrap LITTLEFS to stdio template FILE* stream type  
 * 
 * @details
 *  Wrap LITTLEFS ref and inherite FS, this design for porting fs for different APP
 * @classname pstd stdio interface for upper layer 
 * @instance g_fsio(LittleFS) global instance for filesystem access
 * 
 * @date March 15, 2024
 */
#include "fileio.h"

        pstd::pstd(fs::LittleFSFS& littlefs):littlefsRef(littlefs){// wrap parents LITTLEFS
            for(int i=0;i<10;i++) isEmpty[i]=true;
        }
        pstd::~pstd(){}
        bool pstd::init(){
            initReady=littlefsRef.begin(FORMAT_LITTLEFS_IF_FAILED);
            if (!initReady) {
                Serial.println("Error:LittleFS init fail...");
                return false;             
            }
                Serial.println("pstd:LittleFS init Done...");
            return initReady;
        } 
        void pstd::driver(const char *drv){} // reserved for SD
        FILE* pstd::setFile(File fref){
            for(int i=0;i<10;i++){
                if (isEmpty[i]) {
                    fileArray[i] = fref; 
                    isEmpty[i]=false;
                    return (FILE*)(i+1);
                }
            }
            return 0;
        }
        File pstd::getFile(FILE* stream){
            int idx = ((int) stream)-1;
                return fileArray[idx%10];
        }
        File pstd::closeFile(FILE* stream){
            int idx = ((int) stream)-1;
            File ref = fileArray[idx%10];
            isEmpty[idx%10] = true;
                return ref;
        }
        int pstd::fclose(FILE *stream){
              File fref=closeFile(stream);
              fref.close();
            return 1;
        }
        FILE *pstd::fopen(const char *filename, const char *mode){
            String paths =  getfullpathFileOrDir(LittleFS, filename);
#ifdef ESP32
            if (String(mode)=="rb"){
                  File file = littlefsRef.open((paths).c_str());
                  if (!file&&file.isDirectory())  return NULL;
                  FILE* idx=setFile(file);
                  return idx;
            }else{
                  File file = littlefsRef.open((paths).c_str(),mode,true);
                  Serial.printf("fileopen%x",file);
                  if (!file&&file.isDirectory())  return NULL;
                  FILE* idx=setFile(file);
                  Serial.printf("fileopen%x",file);
                  Serial.printf("filefsFile%x\n",fileArray[(int)idx]);
                  return idx;
            }
            return 0;
#else
            File file = littlefsRef.open((paths).c_str(), mode);
            if (file.isDirectory())  return NULL;
                  FILE* idx=setFile(file);
                  return idx;
#endif      
        
        }
        size_t pstd::fread(char *ptr, size_t size, size_t nmemb, FILE *stream){
              File fref=getFile(stream);
              Serial.printf("fileread%x\n",fref);
              return fref.read((uint8_t *)ptr, size*nmemb);   
        }
        size_t pstd::fwrite(const char  *ptr, size_t size, size_t nobj, FILE *stream){
              File fref=getFile(stream);
              return fref.write((uint8_t *)ptr, size*nobj);   
        }
        int pstd::fseek(FILE *stream, long int offset, int whence){
              File fref=getFile(stream);
              SeekMode mode = (whence==0)? SeekSet:(whence==1)?SeekCur:SeekEnd;
              Serial.printf("fileseek%x\n",fref);
              return fref.seek(offset, mode);   
        }
        long int pstd::ftell(FILE *stream){
              File fref=getFile(stream);
              return fref.position(); 

        }
        int pstd::remove(const char *filename){
            String paths =  getfullpathFileOrDir(LittleFS, filename);
            return littlefsRef.remove(paths);
        }
        int pstd::rename(const char *old_filename, const char *new_filename){
            String pathfrom =  getfullpathFileOrDir(LittleFS, old_filename);
            String pathto =  getfullpathFileOrDir(LittleFS, new_filename);
            return littlefsRef.rename(pathfrom,pathto);
        }
        bool pstd::mkdir(const char *path){
            String pathmk =  getfullpathFileOrDir(LittleFS, path);
            return littlefsRef.mkdir(pathmk);
        }
        bool pstd::rmdir(const char *path){
            String pathrmdir =  getfullpathFileOrDir(LittleFS, path);
            return littlefsRef.rmdir(pathrmdir);
        }      
      //   int fprintf(File *stream, const char *format, ...)
      //   int printf(const char *format, ...)
      //   int sprintf(char *str, const char *format, ...)
      //   int vfprintf(File *stream, const char *format, va_list arg)

pstd g_fsio(LittleFS);
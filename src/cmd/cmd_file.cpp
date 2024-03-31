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
#include "ESP32inc.h"
#include "cmdconfig.h"
int cmd_cd(int argc,char * argv[]){
  if (argc<=1){
      wsTextPrintln(changeDir(LittleFS,"."));
      return 0;
  } 
  wsTextPrintln(changeDir(LittleFS,argv[1]));
  return 0;

}
int cmd_ls(int argc,char * argv[]){
  String path=String(argv[1]);
  int level=1;
  if (argc>=3 && atoi(argv[2])>level) level =atoi(argv[2]);
  wsTextPrintln(listDir(LittleFS, argv[1], level));
  return 0;

}
int cmd_rm(int argc,char * argv[]){
  if (argc<=1) return -1;
#ifdef LITTLEFSFUN
  deleteFile(LittleFS, argv[1]);
#else
  return g_fsio.remove( argv[1]);
#endif
    return 0;

}

int cmd_cat(int argc,char * argv[]){
  if (argc<=1) return -1;

#ifdef LITTLEFSFUN
  wsTextPrint(readFile(LittleFS, argv[1]));
  wsTextPrintln("\n");
#else

  FILE *fid=g_fsio.fopen(argv[1],"r");
  if (fid==NULL) {
    wsTextPrintf("file not found %s\n",argv[1]);
    return 1;
  }
  g_fsio.fseek(fid,0L,2); // seek end
  long int len=g_fsio.ftell(fid);
    wsTextPrintf("file read %s %d\n",argv[1],len);

  char buf[129]; 
  g_fsio.fseek(fid,0L,0); // seek start
  while(len>0) {
     int size=g_fsio.fread(buf,1,128,fid);
     wsSerial.write(buf,size); len-=size;
  }
  g_fsio.fclose(fid);
#endif
    return 0;

}
int cmd_cp(int argc,char * argv[]){
  if (argc<=2) return -1;
  String data = readFile(LittleFS, argv[1]);
  writeFile(LittleFS, argv[2], data.c_str());
  return 0;
}
int cmd_mv(int argc,char * argv[]){
  if (argc<=2) return -1;
  int ret = cmd_cp(argc,argv);
  if (!ret) return ret;
  ret = cmd_rm(argc,argv);
  return ret;
}

#ifdef CMD_FILE_EXT

int cmd_pwd(int argc,char * argv[]){
    wsTextPrintln(currentDir());
    return 0;
}

int cmd_fp(int argc,char * argv[]){
  if (argc<=1) return -1;
  wsTextPrintln(getfullpathFileOrDir(LittleFS, argv[1]));
  return 0;
  
}
int cmd_append(int argc,char * argv[]){
  if (argc<=2) return -1;
  appendFile(LittleFS, argv[1], argv[2]);
  String data = readFile(LittleFS, argv[1]);
  wsTextPrintln(data.c_str());
   return 0;
 
}
int cmd_write(int argc,char * argv[]){
  if (argc<=2) return -1;
      writeFile(LittleFS, argv[1], argv[2]);
      String data = readFile(LittleFS, argv[1]);

   wsTextPrintln(data.c_str());
   return 0;
 
}


int cmd_ls0(int argc,char * argv[]){
  String path=currentDir();
  wsTextPrintln(listDir(LittleFS, path.c_str(), 1));
    return 0;

}

int cmd_mkdir(int argc,char * argv[]){
  if (argc<=1) return -1;
#ifdef LITTLEFSFUN
  createDir(LittleFS, argv[1]);
#else
  g_fsio.mkdir( argv[1]);
#endif
    return 0;

}
int cmd_rmdir(int argc,char * argv[]){
  if (argc<=1) return -1;
#ifdef LITTLEFSFUN
  removeDir(LittleFS, argv[1]);
#else
  g_fsio.rmdir( argv[1]);
#endif
    return 0;

}




#endif // CMD_FILE_EXT

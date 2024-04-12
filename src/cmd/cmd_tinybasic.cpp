
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

#include "command.h"
#ifdef TINYBASIC

int cmd_tinybasic(int argc,char * argv[]){
  int i=0;
  char fbuf[32];
 //WSTransferBuffer(&i); // taskIO
      // WSTransferBufferTaskInit(0);
      // tbsetup();
      if (argc>1) {
          // check externsion exist or not 
          String fname = argv[1];
          String dot =".";
          if (fname.indexOf(dot) == -1) {
              fname+=".bas";
              strcpy(fbuf,fname.c_str());
              argv[1]=fbuf;
          }
      }
      int ret = tbmain(argc,argv);
      
      // WSTransferBufferTaskDestroy();
      return ret;
}
#endif //TINYBASIC

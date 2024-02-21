/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "ESP32_8266.h"
// #include <stdio.h>

void dumpArgList(int argc,char * argv[]){
     for(int i=0;i<argc;i++){
       Serial.printf("argv[%d]=%s,len=%d\n",i,String(argv[i]),String(argv[i]).length());
       for(int j=0;j<String(argv[i]).length();j++){
          Serial.printf("[%2x]%c\n",argv[i][j],argv[i][j]);
       }
     }
}
bool option(int argc,char * argv[],int argcMax){
 return true;
}
int test_main(int argc,char * argv[])
{
  char buf[256];
      sprintf(buf,"2:test main argc=%d\n",argc);
      WSSendTXT(String(buf));
     for(int i=0;i<argc;i++){

       sprintf(buf,"2:argv[%d]=%s\n",i,String(argv[i]));
       WSSendTXT(String(buf));

     }  
     for(int i=0;i<1000;i++){
       sprintf(buf,"2:%d\n",i);
       if (!WSSendTXTAck(String(buf))) {
          WSSendTXT(String(buf));
          break;
       }

     }      //  FILE *fid=fopen("test.txt","w+");
     
    // //  fprintf(fid,"Hello World!");
    //  fclose(fid);

    //  fid=fopen("text.txt","r");
    //  fread(buf,1,100,fid);
    //  sprintf(buf,"read data%s\n",buf);
    //  WSSendTXT(String(buf));

     return 0;

}
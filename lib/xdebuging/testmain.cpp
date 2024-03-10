/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "ESP32inc.h"
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
  char buf[64];

     wsTextPrintf("argc=%d\n",argc);
     for(int i=0;i<argc;i++){
       wsTextPrintf("argv[%d]=%s\n",i,argv[i]);
     }  

     /* test websocket lose or error */
     g_Loopback_Queue.clear();
     int sum=0;
     for(int i=0;i<1000;i++){
      sprintf(buf,"2:%d",i); sum+=i;
      WSSendTXTAck(String(buf));
     }      
     sum *= -1;
     sprintf(buf,"2:%d",sum); 
     WSSendTXTAck(String(buf));
     unsigned long  lastMillis=millis();
     wsTextPrintf("Loopback now(%u)\n",lastMillis);

     while(g_Loopback_Queue.empty() &&(millis()-lastMillis)>30000); // waiting 30s for respones
     if (g_Loopback_Queue.empty()){
            wsTextPrintf("Loopback Test FAIL(Timeout)\n");
            return -1;
     }
      String checksum = g_Loopback_Queue.front();
      g_Loopback_Queue.pop_front();
      if (checksum!="0") {
          wsTextPrintf("Loopback Test Fail checksum(%d)!\n",checksum.toInt());
          return -1;
      }     
      wsTextPrintf("Loopback Test PASS!(%u)\n",(millis()-lastMillis));
    //  FILE *fid=fopen("test.txt","w+");
     
    // //  fprintf(fid,"Hello World!");
    //  fclose(fid);

    //  fid=fopen("text.txt","r");
    //  fread(buf,1,100,fid);
    //  sprintf(buf,"read data%s\n",buf);
    //  WSSendTXT(String(buf));

     return 0;

}


#include <memory.h>
// Size of the buffer
#define BUFFER_SIZE 1024
typedef int (*HELLOPTR)(int,int);
// Allocate memory in IRAM
IRAM_ATTR void* allocateIRAM(size_t size) {
  return malloc(size);
}

int hello(int x,int y){
   return x+y;
  endxxxxx:
  return 0;
};

// Function to execute the hello function copied to IRAM
void executeHelloFunction() {

char sbuf[256];
  HELLOPTR helloAddress = hello;
  int len =0x60;
  sprintf(sbuf,"Executing hello function...%x=%d",(unsigned int)(helloAddress),len);
  wsTextPrintln(sbuf);

  // Allocate memory in IRAM
  HELLOPTR helloFunction = (HELLOPTR)allocateIRAM(len);
  if (!helloFunction) {
    sprintf(sbuf,"Failed to allocate memory");
    wsTextPrintln(sbuf);
    return;
  }
  memcpy((char *)helloFunction, (char *)(helloAddress), len);
    sprintf(sbuf,"copy to allocate memory%x",(unsigned int)helloFunction);
    wsTextPrintln(sbuf);
  // Execute the hello function
  int result = helloFunction(2,3);
    sprintf(sbuf,"2+3=%d",result);
    wsTextPrintln(sbuf);
  // Free allocated memory in IRAM
  free((char *)helloFunction);
}


int cmd_exehello(int argc,char * argv[]) {
    executeHelloFunction();
    return 0;
}

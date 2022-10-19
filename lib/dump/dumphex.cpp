
/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 *
 *  This dump will dump program memory 
 *  Usage: dump address len 
 *  example:
 *      dump 0x400d21a4  256
 *
 *
 *
 *
 */

#include <stdio.h>
#include <ctype.h>
unsigned long  toUlong(const char *num){
    if (num[0]=='0' && num[1]=='x' ){
        return strtoul(num, 0, 16);
    }
    if (isdigit(num[0]) && (num[1]!=0 && isdigit(num[1]))){
        return strtoul(num, 0, 10);

    }
    return 0;
}

void wsTextPrintCstr(const char *msg); // from WebServer.h
// Align memory addess x to an even page
#define ALIGN_UP(x) ((unsigned char*)(((unsigned int)(x + 1) >> 1) << 1))
#define ALIGN_DOWN(x) ((unsigned char*)(((unsigned int)x >> 1) << 1))
int printhexLong(const char *buf, unsigned int len){
    char linebuf[64];
    unsigned long *p=(unsigned long *)ALIGN_DOWN(buf);
    while(len>0){
        sprintf(&linebuf[0],"%08lx ",(unsigned long)p);  // address
        sprintf(&linebuf[9],"%08lx ",(unsigned long)p[0]); // data[0]
        sprintf(&linebuf[9+9],"%08lx ",(unsigned long)p[1]); 
        sprintf(&linebuf[9+9+9],"%08lx ",(unsigned long)p[2]); 
        sprintf(&linebuf[9+9+9+9],"%08lx ",(unsigned long)p[3]);  
        char *cbuf = &linebuf[9+9+9+9+9];
        char *cp = (char *)p[0];                      
        for(int i=0;i<16;i++) {
            cbuf[i] = isprint(*cp) ? (*cp):'.'; cp++;
        }
        linebuf[9+9+9+9+9+16] = '\n';
        linebuf[9+9+9+9+9+17] = 0;
        wsTextPrintCstr(linebuf); p+=4; len-=16;
    }
    return 0;
}
int cmd_dumpHex(int argc,char * argv[]){
 
    const char * address = (const char *)toUlong(argv[1]);
    unsigned long length = toUlong(argv[2]);

    while(length>0) {
        printhexLong(address,length); length-=16;
    }
    

    return 0;
}

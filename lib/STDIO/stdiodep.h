#ifndef __STDIODEP_H__
#define __STDIODEP_H__
#include <Arduino.h>

#undef _FREERTOS_QUEUE_ // dependent on FreeRTOS spuggy0919
#ifdef _FREERTOS_QUEUE_
extern QueueHandle_t queueTX;

#endif

#define BUFFERSIZE 512

void dump(char * buf,int len );

void _d_flushIOBuf();

bool _d_istxready();

int _d_PutChar(char c);
int _d_GetPeek();

// ESP32 <<rx<< Client 
int _d_isrxavailable();
bool _d_insertrxdata(const char* data, int len);
bool _d_fetchrxdata(char *c);
int _d_GetChar();

// Client >>WS>> SERVER 

// ESP32 >>tx>> Client 
bool _d_inserttxdata(unsigned char* data, int len);
bool _d_fetchtxdata(char *c);
#define LINE_LIMIT 256
bool _d_puttxline(char *buf, int len);
bool _d_getrxline(char *buf, int *len);
bool _d_gettxbuf(char *buf, int *len);

bool stdioRedirector();

#define KEYHIT() (_d_isrxavailable())
#define KEYGET() (_d_GetChar())
#define KEYGETLINE(buf,len) (_d_getrxline((buf),(len)))
#define KEY_CRTL_C 0x3
#define KEY_ESC    0x1b
bool keyNotHit();

#endif //__STDIODEP_H__
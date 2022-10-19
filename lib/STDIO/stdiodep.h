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


int _d_isrxavailable();
bool _d_insertrxdata(const char* data, int len);
bool _d_fetchrxdata(char *c);
int _d_GetChar();
int _d_PutChar(char c);


bool _d_inserttxdata(unsigned char* data, int len);
bool _d_fetchtxdata(char *c);
#define LINE_LIMIT 256
bool _d_puttxline(char *buf, int len);
bool _d_getrxline(char *buf, int *len);
bool _d_gettxbuf(char *buf, int *len);

bool stdioRedirector();



#endif //__STDIODEP_H__
/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "stdiodep.h"

#define _FREERTOS_QUEUE_
#ifdef _FREERTOS_QUEUE_

QueueHandle_t queueTX;
SemaphoreHandle_t muxTX;

#else

static char _d_txbuf[BUFFERSIZE];
static int _d_fronttxidx=-1;
static int _d_endtxidx=-1;
#endif 
static char _d_rxbuf[BUFFERSIZE];
static int _d_frontrxidx=-1;
static int _d_endrxidx=-1;

#ifndef DEBUG
void dump(char * buf,int len ){
    // for(int i=0;i<len;i++)
    //     Serial.printf("[%2x]%c",buf[i],buf[i]);
}
#else
void dump(char * buf,int len ){

}
#endif 


// FILE *
// fdevopen(int (*put)(char, FILE *), int (*get)(FILE *));
// static int serial_putc(const char c, FILE *stream) { return _d_putChar(c, stream); }
// // static int (*serial_getc)(FILE *stream) { return  _d_getChar(stream); }
// static FILE *serial_stream = fdevopen(_d_putChar, NULL);


// FILE *
// fdevopen(int (*put)(char, FILE *), int (*get)(FILE *))
// {
// 	FILE *s;

// 	if (put == 0 && get == 0)
// 		return 0;

// 	if ((s = (FILE *) calloc(1, sizeof(FILE))) == 0)
// 		return (FILE *)0;

// 	s->flags = __SMALLOC;

// 	if (get != 0) {
// 		s->get = get;
// 		s->flags |= __SRD;
// 		if (stdin == 0)
// 			stdin = s;
// 	}

// 	if (put != 0) {
// 		s->put = put;
// 		s->flags |= __SWR;
// 		if (stdout == 0) {
// 			stdout = s;
// 			if (stderr == 0)
// 				stderr = s;
// 		}
// 	}

// 	return s;
// }


void _d_flushIOBuf(){
    _d_frontrxidx=-1;
    _d_endrxidx=-1;
#ifndef _FREERTOS_QUEUE_
    _d_fronttxidx=-1;
    _d_endtxidx=-1;
#else
    xQueueReset(queueTX);
#endif
}
int _d_isrxavailable(){
    if (_d_frontrxidx > _d_endrxidx){
        int ret = (_d_frontrxidx - _d_endrxidx);
        return ret;
    }
    if (_d_frontrxidx < _d_endrxidx){
        int ret = (_d_frontrxidx + BUFFERSIZE - _d_endrxidx);
        return ret;
    }
    return 0;
}

bool _d_insertrxdata(const char*data, int len){

    const char *p = data;
    while(*p&&len>0){
        while ((_d_frontrxidx +1) % BUFFERSIZE == _d_endrxidx) { 
            yield();
            Serial.printf("Error:Recev Buffer Full!\n");

            return false; // loss empty bufer
        }
        _d_frontrxidx++; 
        _d_frontrxidx %= BUFFERSIZE;
        // Serial.printf("i[%2x]%c\n",*p,*p);
        _d_rxbuf[_d_frontrxidx]=(char)*p++; len--;
    }

    return true;
}
bool _d_fetchrxdata(char *c){
    if (_d_isrxavailable()){
        if (_d_frontrxidx == _d_endrxidx) return false; // empty
        _d_endrxidx++; 
        _d_endrxidx %= BUFFERSIZE;
        *c = _d_rxbuf[_d_endrxidx];
        // Serial.printf("f[%2x]%c\n",*c,*c);
        return true;
    }

    return false;
}
void ESP_force_wdt(int minisec);
#ifdef _FREERTOS_QUEUE_

bool _d_istxready(){
     char c;
    //  if(xSemaphoreTake(muxTX, portMAX_DELAY) == pdTRUE) {
        bool ret = xQueuePeek(queueTX, &c, 10); //portMAX_DELAY
        // xSemaphoreGive(muxTX);
        yield();
        return ret;
    //  }
     return false;
  
}

extern int insertDelay; /* in Task*/
extern int fetchDelay;

bool _d_inserttxdata(unsigned char*data, int len){
    int idx=0;
        while(idx<len) {
            // if(xSemaphoreTake(muxTX, 10) == pdTRUE) { //portMAX_DELAY
                ESP_force_wdt(100);
                if (xQueueSend(queueTX, &data[idx], 100)){
                        // Serial.printf("ix()[%2x]%c\n",data[idx],data[idx]);
                        idx++;
                        continue;
                }else{
                Serial.printf("Error:xQueueSend full\n");

                yield(); // buffer full wait 
                if(insertDelay>0) vTaskDelay(insertDelay / portTICK_PERIOD_MS ); 
                }
                // xSemaphoreGive(muxTX);
            // }

        }   
        return (idx!=0);
}
bool _d_fetchtxdata(char *c){     
        if (_d_istxready())  {
        //   if(xSemaphoreTake(muxTX, 10) == pdTRUE) {

            if (xQueueReceive(queueTX, c, 66)){
                // Serial.printf("fx()[%2x]%c\n",*c,*c);
                return true;
            }else{
              Serial.printf("Error:xQueueReceive false\n");
               
            }
            if (fetchDelay) vTaskDelay( fetchDelay / portTICK_PERIOD_MS ); 
            // xSemaphoreGive(muxTX);

            ESP_force_wdt(100);
            yield();
        //   }
        }
        return false;
}

#else
int _d_istxavailable(){
    if (_d_fronttxidx > _d_endtxidx)
        return (_d_fronttxidx - _d_endtxidx);
    if (_d_fronttxidx < _d_endtxidx)
        return (_d_fronttxidx + BUFFERSIZE - _d_endtxidx);
    return 0;
}
bool _d_istxready(){
    return (((_d_fronttxidx+1)% BUFFERSIZE) != _d_endtxidx);
}
bool _d_inserttxdata(unsigned char*data, int len){
    unsigned char *p = data;
    while(*p&&len>0){
        _d_fronttxidx++; 
        _d_fronttxidx %= BUFFERSIZE;
        if (_d_fronttxidx == _d_endtxidx) {
            _d_fronttxidx = -1;
            _d_endtxidx = -1;
            Serial.printf("Error:Trans Buffer Full!\n");
            return false; // loss
        }
        // Serial.printf("itx[%2x]%c",*p,*p);
        _d_txbuf[_d_fronttxidx]=(char)*p++; len--;
    }
    return true;
}
bool _d_fetchtxdata(char *c){
    if (_d_istxavailable()){
        if (_d_fronttxidx == _d_endtxidx) return false; // empty
        _d_endtxidx++; 
        _d_endtxidx %= BUFFERSIZE;
        *c = _d_txbuf[_d_endtxidx];
        // Serial.printf("fetchtx[%2x]%c",*c,*c);

    }
    return true;
}

#endif
int _d_GetPeek(){

    int idx= (_d_endrxidx+1) % BUFFERSIZE; 
    if (!_d_isrxavailable()) return 0;
    int c = (int) _d_rxbuf[idx];
    // Serial.printf("peek()[%2x]%c\n",c,c);
    return c;

}

int _d_GetChar(){
    char c;
    while (!_d_isrxavailable())   yield(); // may be deadlock **WARNING **
    while (_d_fetchrxdata(&c)) yield();
    return c;
}
int _d_PutChar(char c)
{
   return  _d_inserttxdata((unsigned char*)&c,1);
      
}

// int _d_getChar(FILE *fp){
//     char c;
//     while (!_d_isrxavailable()); // may be deadlock **WARNING **
//     _d_fetchrxdata(&c);
//     return c;
// }
// int _d_putChar(char c, FILE *fp)
// {
//    return  _d_inserttxdata((unsigned char*)&c,1);
      
// }
bool _d_puttxline(char *buf, int len){
  
    return _d_inserttxdata((unsigned char*)buf,len);
}


bool _d_getrxline(char *buf, int *len){
    int idx = 0;
    char c;
      while(1){
        if (_d_fetchrxdata(&c)){
            if (idx==BUFFERSIZE-1) {buf[idx] = 0; *len = idx+1; return true;}
            if (c=='\r') continue;
            if (c=='\n' || c=='\r' || c == -1) { 
                buf[idx] = '\n'; idx++; // but argc will wrong BUG
                buf[idx] = 0; 
                *len=idx;
                // Serial.printf("\nline complete %s idx=%d\n",buf,idx);
                return true;
            }else if ( (c == '\xff' || c == '\x08') && (idx)>1) {
   			    (idx)--;
            } else {
                buf[idx]=c; idx++;
            } 
        }
        yield();
      }
    return false;
}
/* false : for getting buffer not empty */
bool _d_gettxbuf(char *buf, int *len){
    int idx = 0;
    char c;
    while (_d_istxready()>0){
            _d_fetchtxdata(&c);
            buf[idx]=c; idx++; buf[idx]=0;
            // Serial.printf("f[%2x]%c\n",c,c);

            if (idx==BUFFERSIZE-1) { *len = idx; return false;}
            if (c=='\n' ) {
                // Serial.printf("line complete %s\n",buf);
                *len = idx;
                return false;
            }
        //    vTaskDelay( 10 / portTICK_PERIOD_MS );  output will slow down
            yield();
    }
   
    return (idx==0); // empty
}
bool firstrun=true;
bool WSTransferBufferTaskInit(int wi);

bool stdioRedirector(){
   Serial.printf("stdioRedirector\n");

    if (!firstrun) {
         _d_flushIOBuf();
        return true;
    }
    firstrun =  false;
    queueTX = xQueueCreate(BUFFERSIZE*8, sizeof( unsigned char ) );
    if(queueTX == NULL){
        Serial.println("Error creating the queue1");
        return false;
    }
    muxTX = xSemaphoreCreateMutex();
    if(muxTX == NULL){
        Serial.println("Error creating the queue1");
                return false;

    }
    WSTransferBufferTaskInit(0);
// stdout = serial_stream;
// printf("Format str from ram\nlong = %15ld\nRam string = %10s\n", 1234567890l , "string from RAM");
    return true;
 
}




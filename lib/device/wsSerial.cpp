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
 * @file wsSerial.cpp
 * @brief  websocker Serial  queue and mutex
 * 
 * @details
 *  when user trigger the websocket  then send key to rxQueue and print to txQueue
 * 
 * @date March 11, 2024
 */

/**
 * @brief wsSerial Tx/Rx queue
 * 
 * @details
 * This function queue  touch events.
 * 
 */
#include "wsSerial.h"
#include "debuglog.h"
#include <Ticker.h>


WebSocketSerial wsSerial;

WebSocketSerial::WebSocketSerial() {

}

WebSocketSerial::~WebSocketSerial() {
}
bool WebSocketSerial::begin() {
    //    bool stdioRedirector();
        DEBUGDRVCHK("[wsSerial]:begin\n");
        // flush();
        return true;
}
#define timeoutDuration 5000
bool  WebSocketSerial::flush()  {
    unsigned long currentMillis = millis();
    wsRxMutex.lock();
    while(wsRxBuf.size())
        wsRxBuf.pop();
    wsRxMutex.unlock();
    wsTxMutex.lock();
    while(wsTxBuf.size()&& (millis()-currentMillis)<timeoutDuration){
        wsRxBuf.pop();
        yield(); // delay(2); 
    }
    wsTxMutex.unlock();
    return (wsTxBuf.size()==0);
}

// Serial API
// from RX read
size_t WebSocketSerial::available() {
        wsRxMutex.lock();
        size_t result = wsRxBuf.size();
        wsRxMutex.unlock();
        yield();
        return result;
}
size_t WebSocketSerial::availableForWrite() {
        wsRxMutex.lock();
        size_t result = WSSERIALQUEUESIZE - wsTxBuf.size();
        wsRxMutex.unlock();
        return result;
}

char WebSocketSerial::read() {
    if (available()!=0) {
        wsRxMutex.lock();
        char c = wsRxBuf.front();
        // Serial.printf("[wsSerial]:read[%2x]%c\n",c,c);
        wsRxBuf.pop();
        wsRxMutex.unlock();
        return c;
    }
    return 0;
}
bool WebSocketSerial::escape(){
    if (available()!=0) { // keyboarinput check
            char c=read();
            if (peek()==0xa) read(); // ahead 0xa give up for jerrycript
            // Serial.printf("[wsSerial]:escape[%2x]%c\n",c,c);
            if (c=='\x03') return true;
            if (c=='\x1b') return true;
            if (c=='q'||c=='Q') return true; // force quit
    }
    return false;
}
char WebSocketSerial::getChar(){
    while (available()==0) ;// keyboarinput check need client to sent immediate ? BUG
            char c=read();
    return c;
}
bool WebSocketSerial::readLine(char *buf, int *len){
    int idx = 0;
    int bufsize = *len;
    char c;
      while(1){
        // if (idx==len) return false; check length or not in Jerryscript
        c=read();
        if (c){
            if (idx==BUFFERSIZE-1) {buf[idx] = 0; *len = idx+1; return true;}
            if (c=='\r') continue;
            if (c=='\n' || c=='\r' || c == -1) { 
                buf[idx] = '\n'; idx++; // but argc will wrong BUG
                buf[idx] = 0; 
                *len=idx;
                DEBUGDRVCHK("[wsSerial]:\nline complete %s idx=%d\n",buf,idx);
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
// to  RX write
size_t WebSocketSerial::write(const char* buf,int len){
    int l=0;
        for(int i=0;i<len;i++){
            if (buf[i]==0) break;
            write(buf[i]);
            // if (wsTxBuf.size()<WSSERIALQUEUESIZE) {
            //             wsTxBuf.push(buf[i]);
            //             wsTxMutex.unlock();
            //             return i;
            // }
        }
    return 0;
}

size_t WebSocketSerial::write(uint8_t c) {
        // if (wsTxBuf.size()<WSSERIALQUEUESIZE) {
        if (1) {

            wsTxMutex.lock();
            DEBUGDRVCHK("[wsSerial]:write[%2x]%c\n",c,c);
            wsTxBuf.push(c);
            wsTxMutex.unlock();
            if (c=='\n') yield();
            return 1;
        }else{
            unsigned long currentMillis = millis();
            while( millis()-currentMillis<5000 ){
                if (wsTxBuf.size()<WSSERIALQUEUESIZE) {
                    wsTxMutex.lock();
                    wsTxBuf.push(c);
                    wsTxMutex.unlock();
                 }
                yield(); // delay(1); 
            }
         
        }
    return 0;
}
char WebSocketSerial::peek() {
        wsRxMutex.lock();
        if (wsRxBuf.size()) {
            char c = wsRxBuf.front();
            wsRxMutex.unlock();
            return c;
        }
        wsRxMutex.unlock();
    return 0;
}
// websocket API
/* false : for getting buffer not empty */

bool WebSocketSerial::popLine(char *buf, int *len){
    int idx = 0;
    char c;
    wsTxMutex.lock();
    while (wsTxBuf.size()>0){
        c=wsTxBuf.front(); wsTxBuf.pop();
        buf[idx]=c; idx++; 
        DEBUGDRVCHK("[wsSerial]:f[%2x]%c\n",c,c);

        if (idx==BUFFERSIZE-1) { 
            *len = idx; buf[idx]=0;
            wsTxMutex.unlock();
            DEBUGDRVCHK("[wsSerial]:ERROR:buff-1");
            return false;
        }
        if (c=='\n' ) {
            *len = idx; buf[idx]=0;
            wsTxMutex.unlock();
            //DEBUGDRVCHK("[wsSerial]:line complete %s\n",buf);
            return false;
        }
    //    vTaskDelay( 10 / portTICK_PERIOD_MS );  output will slow down
        wsTxMutex.unlock();
        yield();
        wsTxMutex.lock();
    }
    wsTxMutex.unlock();
    buf[idx]=0;  
     if (idx!=0)  DEBUGDRVCHK("[wsSerial]:idx=%d\n",idx);
    return (idx==0); // empty
}
char WebSocketSerial::pop() {
        wsTxMutex.lock();
        if (!wsTxBuf.size()) {
            char c = wsTxBuf.front();
            wsTxBuf.pop();
            wsTxMutex.unlock();
            return c;
        }
        wsTxMutex.unlock();
    return 0;
}
size_t WebSocketSerial::push(char c) {
        wsRxMutex.lock();
        if (1) {//wsRxBuf.size()<WSSERIALQUEUESIZE) {
            wsRxBuf.push(c);
            wsRxMutex.unlock();
            return 1;
        }else{
           Serial.printf("[wsSerial]:ERROR:RX Buffer Full\n");
        }
        wsRxMutex.unlock();
    return 0;
}

size_t WebSocketSerial::push(const char* buf,int len){
    int l=0;
        // Serial.print(wsRxBuf.size());
        for(int i=0;i<len;i++){
            // if (wsRxBuf.size()>WSSERIALQUEUESIZE) return i;
            wsRxBuf.push(buf[i]);  
            DEBUGDRVCHK("[wsSerial]:push[%2x]\n ",buf[i]);
            yield();
        }
    return len;
}

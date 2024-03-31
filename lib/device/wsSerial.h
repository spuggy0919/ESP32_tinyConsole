/*
 * This file is part of ESP32_TinyConsole.
 *   Copyright (c) 2022 Spuggy0919, spuggy0919@gmail.com
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
#ifndef _WSSERIAL_H_
#define _WSSERIAL_H_

#include <Arduino.h>
#include <mutex>
#include <queue>
bool WSTransferBufferTaskInit(int wi);

class WebSocketSerial {  // no inherits from arduino print : sccording virtual public Print
public:
    WebSocketSerial();
    ~WebSocketSerial();
    bool begin();
    bool flush();
    size_t available();
    size_t availableForWrite();
    char read();
    char getChar();
    bool readLine(char *buf,int *len);
    bool escape();
    size_t write(uint8_t c);
    size_t write(const char* buf,int len);
    char peek();
    size_t push(char c);
    size_t push(const char *buf, int len); 
    char pop();
    bool popLine(char *buf, int *len); 
    const char* classname="WebSocketSerial";
private:
    
    std::mutex wsTxMutex,wsRxMutex;
    std::queue<char> wsTxBuf;
    std::queue<char> wsRxBuf;
    bool client_immediate=false;
};
#define WSSERIALQUEUESIZE 512
extern WebSocketSerial wsSerial;
#define BUFFERSIZE 512

#endif //_WSSERIAL_H_
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
private:
    std::mutex wsTxMutex,wsRxMutex;
    std::queue<char> wsTxBuf;
    std::queue<char> wsRxBuf;
};
#define WSSERIALQUEUESIZE 512
extern WebSocketSerial wsSerial;
#define BUFFERSIZE 512

#endif //_WSSERIAL_H_
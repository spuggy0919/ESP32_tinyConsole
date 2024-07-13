/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
/*
websocket for tinyconsole protocol is defined below 
Server              Client    description
        heartbeat             
       <----------- P:PING    heartbeat start after 45sec when idle 
P:PONG -----------> P:PING

        server TX
d:MSG  ----------->           d is devive code  0:terminal 1:monitor, 8:canvas, 9:audio
       <----------- A:        t:test

        client TX 
       <----------- V:version protocol version
V:ver  ----------->           echo 
        client key event
       <----------- X:msg     keyboard keyin
X:msg  ----------->           echo 
        client touch event(TBD)
       <----------- T:t:x:y   touch event (TBD)
T:t:x:y----------->           echo 
        client mouse event(TBD)
       <----------- M:k:x:y   mouse event
M:t:x:y----------->           echo 
       <----------- R:        flush keybuf (TBD)
*/
#include "WebServer.h"
// websocket
AsyncWebSocket ws("/ws");

// Define a queue for outgoing WebSocket messages
SemaphoreHandle_t xackSemaphore;

// Define WebSocket state for each connection
#define MAX_WEBSOCKET_CONNECTIONS 10

int ws_clientid;
AsyncWebSocketClient *clientconnects[MAX_WEBSOCKET_CONNECTIONS];
String CurrntClientIP="";
bool isConnect = false;

#define KEY_TERMINAL   0
#define KEY_CONSOLELOG 1


bool WebWSConnect(){
    return isConnect;
}

// WSSendTXT should be low evel format as sno:dev:base64msg
#define PACKETSNO
#ifdef PACKETSNO
int packetsno=0;
#endif
void WSSendTXT(String msg){
    if (!WebWSConnect()) return;
    // Serial.println("WSSendTXT: " + msg);

    ws.text(ws_clientid, msg.c_str(),msg.length()); 

}
// WSSendTXTAck should be low evel format as dev:base64msg
int maxRetries=1;
bool WSSendTXTAck(String msg){
int timeout = (300 / portTICK_PERIOD_MS) + 1; // 100ms  Adjust as needed
    // xSemaphoreTake(xackSemaphore, 0); // anyway clear Given *BUG* race occurred
    if (!WebWSConnect()) return false;

#ifdef PACKETSNO
    String packetsnostr=String(packetsno)+":"; packetsno++;
    msg=packetsnostr+msg;
#endif
    // Serial.println("WSSendTXTAck: " + msg);
    WSSendTXT(msg); 
    

    if (xSemaphoreTake(xackSemaphore, timeout) == pdFALSE) {
        // No acknowledgment received yet, retry sending the message
        for (int attempt = 1; attempt <= maxRetries; ++attempt) {
            // Serial.println("WSSendTXTAck:attempt " + String(attempt) + msg);

            // Send the message to the client
            WSSendTXT(msg);

            // Wait for acknowledgment or retry interval
            unsigned long startMillis = millis();
            if (xSemaphoreTake(xackSemaphore, timeout) == pdTRUE)  {
                break;
            }
            if (attempt == maxRetries) return false;
        }
    }
    return true;

}
#define MESSAGELEN 128
void wsTextPrintBase64(int key,String msg){
  String substr;
  // Serial.println("wsTextPrintBase64:"+String(key));
  if (!WebWSConnect()) return;
   while(msg.length() > 0) {
      if (msg.length()>=MESSAGELEN) {
        substr =  msg.substring(0,MESSAGELEN);
        msg = msg.substring(MESSAGELEN,msg.length());
      }else{
        substr = msg;
        msg = "";
      }
      // Serial.println(substr);

      int encodedLength = Base64.encodedLength(substr.length());
      char encodedString[encodedLength + 1];
       Base64.encode(encodedString, (char *) substr.c_str(), substr.length());      
      //  Serial.printf("str = %s, len = %d, enstr=%s,enlen=%d\n",substr.c_str(),substr.length(),encodedString,encodedLength);
       String keystr=(key==KEY_TERMINAL)? "0:":String(key)+":";
       WSSendTXTAck(keystr+String(encodedString));
       //  events.send(encodedString,"textarea",millis());
      //  vTaskDelay( 50 / portTICK_PERIOD_MS ); 
    }
    return ; // DEBUG

}
void wsTextPrintBase64noAck(int key,String msg){
  String substr;
  if (!WebWSConnect()) return;
  // Serial.println("wsTextPrintBase64noAck: ("+String(key)+")");
   while(msg.length() > 0) {
     if (msg.length()>=MESSAGELEN) {
        substr =  msg.substring(0,MESSAGELEN);
        msg = msg.substring(MESSAGELEN,msg.length());
      }else{
        substr = msg;
        msg = "";
      }
      // Serial.println(substr);

      int encodedLength = Base64.encodedLength(substr.length());
      char encodedString[encodedLength + 1];
       Base64.encode(encodedString, (char *) substr.c_str(), substr.length());      
      //  Serial.printf("str = %s, len = %d, enstr=%s,enlen=%d\n",substr.c_str(),substr.length(),encodedString,encodedLength);
       String keystr=(key==KEY_TERMINAL)? "0:":String(key)+":";
#ifdef PACKETSNO
       String packetsnostr=String(packetsno)+":"; packetsno++;
       String sentmsg=packetsnostr+keystr+String(encodedString);
#else 
       String eentmsg=keystr+String(encodedString);
#endif
       WSSendTXT(sentmsg); 
       //  events.send(encodedString,"textarea",millis());
      //  vTaskDelay( 50 / portTICK_PERIOD_MS ); 
    }
    return ; // DEBUG
}
void wsTextPrint(String msg){
   wsTextPrintBase64(0,msg);
    return ; // DEBUG

}
void wsTextPrintCstr(const char *msg){
   wsTextPrint(String(msg));
}

void wsTextPrintln(String msg){
   wsTextPrintBase64(0,msg+"\n");
  return ; // DEBUG

}
#include <string.h>
#include <mutex>
std::mutex wsTextMutex; 
void wsTextPrintf(const char *fmt,...){
      if (!WebWSConnect()) return;
  // std::lock_guard<std::mutex> lock(wsTextMutex); // avoid reentry 
  int sizebuf = strlen(fmt)+100;
  char *buf=(char *)malloc(sizebuf);
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizebuf, fmt, args);
  va_end(args);  
  wsTextPrintBase64(0,buf); // extension 
  free(buf);
}
void wsMonitorPrintf(const char *fmt,...){
      if (!WebWSConnect()) return;
  // std::lock_guard<std::mutex> lock(wsTextMutex); // avoid reentry 
  int sizebuf = strlen(fmt)+100;
  char *buf=(char *)malloc(sizebuf);
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizebuf, fmt, args);
  va_end(args);  
  wsTextPrintBase64noAck(1,buf); // extension 
  free(buf);
}

// wi is dev, 0:terminal 1:monitor, msg with base64
void WSTransferMessage(int wi,String msg){
    if (wi==0)
      wsTextPrintBase64(wi,String(msg)); 
    else
      wsTextPrintBase64noAck(wi,String(msg)); 


}

void _wsDevice(const char dev,const char *fmt,...){
  int sizebuf = strlen(fmt)+100;
  char *buf=(char *)malloc(sizebuf);  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizebuf, fmt, args);
  va_end(args);  
  wsTextPrintBase64(dev,buf); // extension 
  free(buf);
}

static char obuf[2][BUFFERSIZE];
static int olen[2]={0,0};
String sendstr = "";
std::queue<String> queueStr;
void WSTransferBufferFlush(int wi){

    if (WebWSConnect()){
    

      // Serial.printf("WS:TX:\n");
      while (!wsSerial.popLine(obuf[wi],&(olen[wi]))){
        char c=0xa;
        // for(int i = 0 ;i <olen[wi] ;i++) {
        //     c=obuf[wi][i];
        //       // Serial.printf("f[%2x]\n ",c);
        // } 
        sendstr += String(obuf[wi]);
        // Serial.printf("s%s[%2x]%c\n ",sendstr.c_str(),c,c);
         if (c<=0x7f) {
           String wholeStr="";
           while (queueStr.size()>0) {
                wholeStr+=queueStr.front();
                queueStr.pop();
           }
           wholeStr+=sendstr;
           wsTextPrintBase64(wi,wholeStr); sendstr = ""; olen[wi]=0;
         }
         else { // push sendstr wait for coming
            queueStr.push(sendstr); sendstr="";
         }
         yield();
      }
   }

}

void WSTransferChar(int wi,char c){
      // taskENTER_CRITICAL();
      if (!WebWSConnect()) return;
      if (c=='\x0a'||c=='\x0d'||c=='?'||c=='.'||c=='!') {
        obuf[wi][olen[wi]]='\n'; olen[wi]+=1;
        obuf[wi][olen[wi]]=0;
        wsTextPrintBase64(wi,String(obuf[wi]));  olen[wi]=0;
      }else{
        obuf[wi][olen[wi]]=c; olen[wi]+=1;
      }
      // taskEXIT_CRITICAL();
}


std::list<String> g_Loopback_Queue;

/* parser receive who:command:device:msg */
void wsOnMessageReceive(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String receive = String((const char *)data);
    String cmd =  receive.substring(0,2);
    String msg =  receive.substring(2,receive.length());

    if (cmd == "X:"){ // transfer from client keyboard
      int loc = 0;
      // if (data[2]=='\x0d') loc=3;
        // Serial.printf("WS:RECV:\n");
        // char c;
        // for(int i = 0 ;i <msg.length() ;i++) {
        //     c=msg.charAt(i);
        //       Serial.printf("[websocket]:R[%2x]\n ",c);
        // }

      if (!wsSerial.push((const char*)(msg.c_str()), msg.length())){
        Serial.printf("[websocket]:ERROR: WS:RECV: buffer full!");
      }
      // wsTextPrintBase64noAck(1,"X:"+msg+"\n");

    }

    if (cmd == "A:"){ // ack reply from client
        // wsTextPrintBase64noAck(1,"E:"+msg+"\n");
        // if (msg==String(packetsno)) {
            xSemaphoreGive(xackSemaphore);
        // }
    }
    if (cmd == "2:"){ // loopback test
        g_Loopback_Queue.push_back(msg);
    }

    if (cmd == "V:"){ // version
        // wsTextPrintBase64noAck(1,"V:"+msg+":clientID="+String(ws_clientid)+"\n");
    }
    if (cmd == "T:"){ // touch
        // wsTextPrintBase64noAck(1,"T:"+msg+"\n");
        gTouchQueue.push(msg);
    }
    if (cmd == "M:"){ // mouse
        // wsTextPrintBase64noAck(1,"M:"+msg+"\n");
        gTouchQueue.push(msg);
    }
    if (cmd == "R:"){ // request from client 
        WSTransferBufferFlush(0);
    }
    if (cmd == "P:"){ // request from client  PING PONG
        wsTextPrintBase64noAck(1,"P:PONG");
    }
  }
}


void wsEventHandle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (CurrntClientIP!=""&&CurrntClientIP!=client->remoteIP().toString()) return; // open and only same ip
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      ws_clientid = client->id();
      CurrntClientIP=client->remoteIP().toString();
   //     WSTransferBufferTaskInit(0); // wi is 0 for textlog task sent message for basic
    // Set WebSocket state to CONNECTED when a connection is established
      // clientconnects[client->id()] = client; 
      isConnect=true;
      pwmled(255);
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      // Set WebSocket state to CONNECTED when a connection is established
      isConnect=false;
      CurrntClientIP="";
      pwmled(0);
      break;
    case WS_EVT_DATA:
      // if (client->status()==WS_CONNECTED) {
         wsOnMessageReceive(arg, data, len);
      // }
      isConnect=true;
      break;
    case WS_EVT_PONG:
      Serial.printf("WebSocketPongEvent\n");
      WSTransferMessage(1,"WebSocketPongEvent\n");
      isConnect=true;
        break;
    case WS_EVT_ERROR:
      isConnect=true;

        WSTransferMessage(1,"WebSocketErrorEvent\n");

      break;
  }
}

void WebSocketStart(AsyncWebServer &server) {
  // ... (WiFi connection setup and other initialization)

  // Initialize FreeRTOS semaphore
  xackSemaphore = xSemaphoreCreateBinary();
  // Take the semaphore immediately after creation to set its state to "unavailable"
  xSemaphoreTake(xackSemaphore, 0);

  ws.onEvent(wsEventHandle);
  // Attach WebSocket to the server
  server.addHandler(&ws);
}

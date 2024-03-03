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
AsyncWebSocketClient *CurrntClient=NULL;
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
void wsTextPrintBase64(int key,String msg){
  String substr;
  // Serial.println("wsTextPrintBase64:"+String(key));
  if (!WebWSConnect()) return;
   while(msg.length() > 0) {
      if (msg.length()>=64) {
        substr =  msg.substring(0,64);
        msg = msg.substring(64,msg.length());
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
     if (msg.length()>=64) {
        substr =  msg.substring(0,64);
        msg = msg.substring(64,msg.length());
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
void wsTextPrintf(const char *fmt,...){
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);  
  wsTextPrintBase64(0,buf); // extension 
}
void wsMonitorPrintf(const char *fmt,...){
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);  
  wsTextPrintBase64noAck(1,buf); // extension 
}

// wi is dev, 0:terminal 1:monitor, msg with base64
void WSTransferMessage(int wi,String msg){
    if (wi==0)
      wsTextPrintBase64(wi,String(msg)); 
    else
      wsTextPrintBase64noAck(wi,String(msg)); 


}

void _wsDevice(const char dev,const char *fmt,...){
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);  
  wsTextPrintBase64(dev,buf); // extension 
}

static char obuf[2][BUFFERSIZE];
static int olen[2]={0,0};
void WSTransferBufferFlush(int wi){

    if (WebWSConnect()){
    

      // Serial.printf("WS:TX:\n");
      while (!_d_gettxbuf(obuf[wi],&(olen[wi]))){
        char c;
        for(int i = 0 ;i <olen[wi] ;i++) {
            c=obuf[wi][i];
              // Serial.printf("f[%2x]\n ",c);
        } 
          {wsTextPrintBase64(wi,String(obuf[wi])); olen[wi]=0;}
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
        char c;
        for(int i = 0 ;i <msg.length() ;i++) {
            c=msg.charAt(i);
              Serial.printf("R[%2x]\n ",c);
        }

      if (!_d_insertrxdata((const char*)(msg.c_str()), msg.length())){
        Serial.printf("ERROR: WS:RECV: buffer full!");
      }
      wsTextPrintBase64noAck(1,"X:"+msg+"\n");

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
        wsTextPrintBase64noAck(1,"T:"+msg+"\n");
        gTouchQueue.push(msg);
    }
    if (cmd == "M:"){ // mouse
        wsTextPrintBase64noAck(1,"M:"+msg+"\n");
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
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      ws_clientid = client->id();
      CurrntClient=client;
   //     WSTransferBufferTaskInit(0); // wi is 0 for textlog task sent message for basic
    // Set WebSocket state to CONNECTED when a connection is established
      // clientconnects[client->id()] = client; 
      isConnect=true;
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      // Set WebSocket state to CONNECTED when a connection is established
      isConnect=false;

      break;
    case WS_EVT_DATA:
      if (client->status()==WS_CONNECTED) {
         wsOnMessageReceive(arg, data, len);
      }
      isConnect=true;
      break;
    case WS_EVT_PONG:
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

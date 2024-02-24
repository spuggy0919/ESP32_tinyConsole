#include "WebServer.h"
// websocket
AsyncWebSocket ws("/ws");
// websocket


// Define WebSocket states
enum WebSocketState {
    WS_STATE_CONNECTED,
    WS_STATE_READY,
    WS_STATE_BUSY,
    WS_STATE_DISCONNECTED
};

// Define a queue for outgoing WebSocket messages
QueueHandle_t webSocketQueue;
SemaphoreHandle_t xackSemaphore;
QueueHandle_t messageQueue;

// Structure to hold WebSocket data
struct WebSocketData {
  String message;
};

// Function prototypes
void wsTextTask(void *pvParameters);
void wsEventTask(void *pvParameters);


// Define WebSocket state for each connection
#define MAX_WEBSOCKET_CONNECTIONS 10
WebSocketState webSocketStates[MAX_WEBSOCKET_CONNECTIONS];

#define KEY_TERMINAL   0
#define KEY_CONSOLELOG 1

bool ws_connect=false;
bool WebWSConnect(){
    return ws_connect;
}
int ws_clientid;
void notifyClients(String sliderValues) {
  ws.textAll(sliderValues);
}
int sno=0;
void wsTextPrintBase64(int key,String msg){
  String substr;
  //  Serial.println("wsTextPrintBase64");
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
       String snostr=String(sno)+":";
       WSSendTXTAck((keystr+String(encodedString)).c_str());
       //  events.send(encodedString,"textarea",millis());
      //  vTaskDelay( 50 / portTICK_PERIOD_MS ); 
    }
    return ; // DEBUG

}
void wsTextPrintBase64noAck(int key,String msg){
  String substr;
  //  Serial.println("wsTextPrintBase64");
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
       String snostr=String(sno)+":";
       WSSendTXT((keystr+String(encodedString)).c_str());
       //  events.send(encodedString,"textarea",millis());
      //  vTaskDelay( 50 / portTICK_PERIOD_MS ); 
    }
    return ; // DEBUG

}
void wsTextPrintCstr(const char *msg){
   wsTextPrint(String(msg));
}
void wsTextPrint(String msg){
   wsTextPrintBase64(0,msg);
    return ; // DEBUG

}
void wsTextPrintln(String msg){
   wsTextPrintBase64(0,msg+"\n");
  return ; // DEBUG

}

// void wsPrintf(const char *fmt,...){
//   char buf[128];
//   va_list args;
//   va_start(args, fmt);
//   vsnprintf(buf, sizeof(buf), fmt, args);
//   va_end(args);  wsTextPrintBase64(9,buf); // extension 
// }
void _wsDevice(const char dev,const char *fmt,...){
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);  wsTextPrintBase64(dev,buf); // extension 
}
// void WSSendTXT(String msg);

static char obuf[2][BUFFERSIZE];
static int olen[2]={0,0};
void WSTransferBufferFlush(int wi){

   if (ws_connect){
    

      // Serial.printf("WS:TX:\n");
      while (!_d_gettxbuf(obuf[wi],&(olen[wi]))){
        char c;
        for(int i = 0 ;i <olen[wi] ;i++) {
            c=obuf[wi][i];
              Serial.printf("f[%2x]\n ",c);
        } 
          {wsTextPrintBase64(wi,String(obuf[wi])); olen[wi]=0;}
         yield();
      }
   }

}

void WSTransferChar(int wi,char c){
      // taskENTER_CRITICAL();
      if (c=='\x0a'||c=='\x0d'||c=='?'||c=='.'||c=='!') {
        obuf[wi][olen[wi]]='\n'; olen[wi]+=1;
        obuf[wi][olen[wi]]=0;
        wsTextPrintBase64(wi,String(obuf[wi]));  olen[wi]=0;
      }else{
        obuf[wi][olen[wi]]=c; olen[wi]+=1;
      }
      // taskEXIT_CRITICAL();


}

// msg with base64
void WSTransferMessage(int wi,String msg){

      wsTextPrintBase64(wi,String(msg)); 


}
// low evel sent 
void WSSendTXT(String msg){
      ws.text(ws_clientid, msg.c_str(),msg.length());

}
// low evel sent 
int maxRetries=3;
bool WSSendTXTAck(String msg){
int timeout = (300 / portTICK_PERIOD_MS) + 1; // 100ms  Adjust as needed
    // xSemaphoreTake(xackSemaphore, 0); // anyway clear Given *BUG* race occurred

    ws.text(ws_clientid, msg.c_str(),msg.length());

    if (xSemaphoreTake(xackSemaphore, timeout) == pdFALSE) {
        // No acknowledgment received yet, retry sending the message
        for (int attempt = 1; attempt <= maxRetries; ++attempt) {
            Serial.println("Retrying message, attempt " + String(attempt) + msg);

            // Send the message to the client
            ws.text(ws_clientid, msg.c_str(),msg.length());

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
        Serial.printf("WS:RECV:\n");
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
        // wsTextPrintBase64(1,"A:"+msg+"\n");
        // if (msg==String(sno)) {
            xSemaphoreGive(xackSemaphore);
        // }
    }
    if (cmd == "V:"){ // version
        wsTextPrintBase64noAck(1,"V:"+msg+":clientID="+String(ws_clientid)+"\n");
    }
    if (cmd == "T:"){ // touch
        wsTextPrintBase64noAck(1,"T:"+msg+"\n");
    }
    if (cmd == "M:"){ // mouse
        wsTextPrintBase64noAck(1,"M:"+msg+"\n");
    }
    if (cmd == "R:"){ // request from client 
        WSTransferBufferFlush(0);
    }
    if (cmd == "P:"){ // request from client  PING PONG
        wsTextPrintBase64noAck(1,"P:PONG");
    }
  }
}


// Function to send WebSocket data
void sendWebSocketData(AsyncWebSocketClient *client, const char *data) 
{
    // Get the WebSocket state
    WebSocketState state = webSocketStates[client->id()];

    // Check the state and handle accordingly
    while (state == WS_STATE_BUSY|| state == WS_STATE_DISCONNECTED ) delay(10); // wait for time out
    if (state == WS_STATE_READY) {
        // Enqueue the data and change the state to BUSY
        xQueueSend(webSocketQueue, data, portMAX_DELAY);
        webSocketStates[client->id()] = WS_STATE_BUSY;
    } 
}

// Dequeue and send WebSocket data
void processWebSocketQueue(AsyncWebSocketClient *client) 
{
    while (1) {
        // Dequeue data from the WebSocket queue
        char data[256];
        if (xQueueReceive(webSocketQueue, data, portMAX_DELAY) == pdTRUE) {
            // Send the data over the WebSocket connection

            // Change the WebSocket state back to READY when the queue is empty
            if (uxQueueMessagesWaiting(webSocketQueue) == 0) {
                webSocketStates[client->id()] = WS_STATE_READY;
            }
        }
    }
}

// Function to receive WebSocket data
void receiveWebSocketData(AsyncWebSocketClient *client, const char *data) 
{
    // Get the WebSocket state
    WebSocketState state = webSocketStates[client->id()];

    // Check the state and handle accordingly
    if (state == WS_STATE_READY) {
        // Enqueue the data and change the state to BUSY
        xQueueSend(webSocketQueue, data, portMAX_DELAY);
        webSocketStates[client->id()] = WS_STATE_BUSY;
    } 
}

// WebSocket connection callback
// void onWebSocketDisConnect(AsyncWebSocket *server, AsyncWebSocketClient *client) 
// {
//     // Set WebSocket state to CONNECTED when a connection is established
//     webSocketStates[client->id()] = WS_STATE_DISCONNECTED;
// }

void wsEventHandle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      ws_clientid = client->id();
      ws_connect = true;
   //     WSTransferBufferTaskInit(0); // wi is 0 for textlog task sent message for basic
    // Set WebSocket state to CONNECTED when a connection is established
    webSocketStates[client->id()] = WS_STATE_CONNECTED;  

      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      ws_connect = false;
      // Set WebSocket state to CONNECTED when a connection is established
      webSocketStates[client->id()] = WS_STATE_DISCONNECTED;

      break;
    case WS_EVT_DATA:

      wsOnMessageReceive(arg, data, len);
      break;
    case WS_EVT_PONG:
        WSTransferMessage(1,"WebSocketPongEvent\n");
        break;
    case WS_EVT_ERROR:
        WSTransferMessage(1,"WebSocketErrorEvent\n");

      break;
  }
}



void wsTextTask(void *pvParameters) {
  while (true) {
    // Wait for WebSocket text messages
    WebSocketData webSocketData;
    if (xQueueReceive(messageQueue, &webSocketData, portMAX_DELAY)) {
      // Process the WebSocket text message
      Serial.println("Received WebSocket message: " + webSocketData.message);

      // Simulate acknowledgment or other processing
      // ...

      // Notify the other task (wsEventTask) that processing is complete
      xSemaphoreGive(xackSemaphore);
    }
  }
}

void wsEventTask(void *pvParameters) {
  while (true) {
    // Wait for acknowledgment with a timeout
    if (xSemaphoreTake(xackSemaphore, pdMS_TO_TICKS(5000)) == pdTRUE) {
      Serial.println("Ack received within timeout!");
    } else {
      Serial.println("Timeout: Ack not received! Retrying...");
      
      // Implement retry mechanism here
      // ...

      // Notify the other task (wsTextTask) that the retry is done
      xSemaphoreGive(xackSemaphore);
    }
  }
}

void WebSocketStart(AsyncWebServer &server) {
  // ... (WiFi connection setup and other initialization)

  // Initialize FreeRTOS semaphore
  xackSemaphore = xSemaphoreCreateBinary();
  // Take the semaphore immediately after creation to set its state to "unavailable"
  xSemaphoreTake(xackSemaphore, 0);
  // Create a FreeRTOS queue for WebSocket messages
  messageQueue = xQueueCreate(10, sizeof(WebSocketData));

//   // WebSocket text event handler
//   ws.onText([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
//     String receivedMessage(reinterpret_cast<char*>(data), len);

//     // Enqueue the message for processing
//     WebSocketData webSocketData;
//     webSocketData.message = receivedMessage;

//     if (xQueueSendToBack(messageQueue, &webSocketData, 0) != pdPASS) {
//       Serial.println("Failed to enqueue WebSocket message");
//     }
//   });



  // Start FreeRTOS tasks
//   xTaskCreate(wsTextTask, "wsTextTask", 4096, NULL, 1, NULL);
//   xTaskCreate(wsEventTask, "wsEventTask", 4096, NULL, 1, NULL);

//   // WebSocket text event handler
//   ws.onText(wsTextHandle);
  // WebSocket event handler callback
  ws.onEvent(wsEventHandle);
  // Attach WebSocket to the server
  server.addHandler(&ws);
}

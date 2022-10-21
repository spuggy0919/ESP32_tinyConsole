/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include <Webserver.h>
#include "Base64.h"
#include <Freertos/FreeRTOS.h>

#include <freertos/task.h>
#include <esp_system.h>
// bool dummy(){return false;}

// bool (*WebserForInterpreterSendCmd)()= dummy;

// Create AsyncWebServer object on port 80
AsyncWebServer  server(80);
// Create an Server Sent Event (SSE) Source on /events
AsyncEventSource events("/events");
// websocket
AsyncWebSocket ws("/ws");
// websocket

#define KEY_TERMINAL   0
#define KEY_CONSOLELOG 1

bool ws_connect=false;
bool WebWSConnect(){
    return ws_connect;
}
int ws_clientid ;
void notifyClients(String sliderValues) {
  ws.textAll(sliderValues);
}
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
       String keystr=(key==KEY_TERMINAL)? "0:":"1:";
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

void WSSendTXT(String msg);

static char obuf[2][BUFFERSIZE];
static int olen[2]={0,0};
void WSTransferBufferFlush(int wi){

   if (ws_connect){
    

      // Serial.printf("WS:TX:\n");
      while (!_d_gettxbuf(obuf[wi],&(olen[wi]))){
        char c;
        for(int i = 0 ;i <olen[wi] ;i++) {
            c=obuf[wi][i];
              Serial.printf("[%2x]\n ",c);
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
/* parser receive who:command:device:msg */
void WebSocketMessageReceive(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String receive = String((const char *)data);
    String cmd =  receive.substring(0,2);
    String msg =  receive.substring(2,receive.length());

    if (cmd == "X:"){ // transfer from client 
      int loc = 0;
      // if (data[2]=='\x0d') loc=3;
        Serial.printf("WS:RECV:\n");
        char c;
        for(int i = 0 ;i <msg.length() ;i++) {
            c=msg.charAt(i);
              Serial.printf("[%2x]\n ",c);
        }

      if (!_d_insertrxdata((const char*)(msg.c_str()), msg.length())){
        Serial.printf("ERROR: WS:RECV: buffer full!");
      }
    }
    if (cmd == "R:"){ // request from client 
        WSTransferBufferFlush(0);
    }
    if (cmd == "P:"){ // request from client 
        WSTransferMessage(1,msg);
    }
  }
}
void WSEventHandle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      ws_clientid = client->id();
      ws_connect = true;
   //     WSTransferBufferTaskInit(0); // wi is 0 for textlog task sent message for basic
     

      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
       ws_connect = false;

      break;
    case WS_EVT_DATA:

      WebSocketMessageReceive(arg, data, len);
      break;
    case WS_EVT_PONG:
        WSTransferMessage(1,"WebSocketPongEvent\n");
        break;
    case WS_EVT_ERROR:
          WSTransferMessage(1,"WebSocketErrorEvent\n");

      break;
  }
}

void WebSocketStart() {
  ws.onEvent(WSEventHandle);
  server.addHandler(&ws);
}

//http
void webEventTask(){
    // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    Serial.println("events.onConnect ...");
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    // client->send("hello!", NULL, millis(), 10000);
#ifdef ESP32 
    // for ESP32 RTC
    client->send(String(timerCurrent()).c_str(),"currenttime",millis(), 8000);
#endif
    // client->send(pwmledState.c_str(),"gpiostate",millis());

  });
  server.addHandler(&events);  // Handle Web Server Events
}
void eventDownloadFile(String msg) {
    events.send(String(msg).c_str(),"Eventdoownload",millis(), millis());
}
void eventSend(String msg,const char * key) {
    events.send(String(msg).c_str(), key,millis(), millis());
}


// 1. Replaces placeholder %STATE% with LED state value
// 2. using SSE to change it. (now)
String processor(const String& var) {
  // if(var == "STATE") {
  //   events.send(pwmledState.c_str(),"gpiostate",millis());
  //   return pwmledState;
  // }
  // if(var == "CHARTDATA") {
  //   return pwmlogData(); // placeholder %CHARTDATA% in index.htm
  // }
  // if(var == "CURRENTTIME") {
  //   events.send(timerCurrent().c_str(),"currenttime",millis());
  //   return timerCurrent();
  // }
  // if (var == "SLIDERVALUE"){
  //   return String(pwmledr());
  // } 
  return String();
}

// Web HTTP Server
void notFoundPage(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "404 Not Found");
}
#define OK200() request->send(200, "text/plain", "200 OK")

String dumpParams(AsyncWebServerRequest *request){
  String plist = "/param?";
  for(unsigned int i=0;i< request->params();i++){
      AsyncWebParameter *p = request->getParam(i);
      Serial.print("Param name: "); 
      Serial.println(p->name()); plist += p->name() +"=" ;
      Serial.print("Param value: ");
      Serial.println(p->value()); plist += p->value();
      if (i !=  request->params()-1) plist +=  "&";
  }
  return plist;
}
String indexPath = "/index.htm";
// handles uploads
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  Serial.println(logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object
    request->_tempFile = LittleFS.open("/" + filename, "w");
    Serial.println(logmessage);
  }

  if (len) {
    // stream the incoming chunk to the opened file
    request->_tempFile.write(data, len);
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    Serial.println(logmessage);
  }

  if (final) {
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    // close the file handle as the upload is now done
    request->_tempFile.close();
    Serial.println(logmessage);
    request->redirect("/");
  }
}

void WebServerPage()
{

  server.serveStatic("/", LittleFS, "/");  // for style.css load
  // Route to set GPIO state to LOW
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    wsTextPrintln("HTTP_GET / ");
    request->send(LittleFS, indexPath, "text/html", false, processor);
  });
 
  // run handleUpload function when any file is uploaded
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
      }, handleUpload);

  // Send a GET request to <ESP_IP>/param?value=<inputMessage>
  // ex. <ESP_IP>/param?field1=value1&field2=value2&field3=value3...
  server.on("/param", HTTP_POST, [] (AsyncWebServerRequest *request) {
    String value = "";
    // text loger can input enter key to send txt line

    if (request->hasParam("cmd")) {
      value = request->getParam("cmd")->value();
      Serial.println("recv String:"+value);
      // value = "cmd=" + value;
      // (*WebserForInterpreterSendCmd)(value);  send by HTTP POST deprecated
      // events.send(value.c_str(),"textarea",millis());
    }
    if (request->hasParam("slider")) {
      value = request->getParam("slider")->value();
      pwmled(value.toInt()); // fpr ESP32 pwm led 
      Serial.println("pwm value for LED:"+value);
      value = "slider=" + value;
    }
#ifdef ESP32
  // ESP32 set RTC with System Timer
    if (request->hasParam("timer")) {
      value = request->getParam("timer")->value();
      if (value != ""){
        // Serial.println("clientTime="+clientTime);
        // set RTC epoch 
        String timeinsec=value.substring(0,value.length()-3);
        unsigned long t = atol(timeinsec.c_str());
        // Serial.println(t);
        timerSetEpoch(t);
        value = "timer=" + value;
      }
    }
#endif
    if (value == "") {
       value = dumpParams(request);
    }
    // wsTextPrintln("HTTP_POST/param?"+value);
    Serial.println(dumpParams(request));

    OK200();
  });
  // 4. Set up Server Sent Event
  WebSocketStart();
  webEventTask();

  server.begin();
  Serial.println("WebServerGPIOControl Starting..."); 

}
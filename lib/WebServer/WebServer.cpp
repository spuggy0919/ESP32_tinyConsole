/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "WebServer.h"

// bool dummy(){return false;}

// bool (*WebserForInterpreterSendCmd)()= dummy;

// Create AsyncWebServer object on port 80
AsyncWebServer  server(80);
// Create an Server Sent Event (SSE) Source on /events
AsyncEventSource events("/events");





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
  WebSocketStart(server);
  webEventTask();

  server.begin();
  Serial.println("WebServerGPIOControl Starting..."); 

}
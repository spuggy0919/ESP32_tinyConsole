#include "ESP32inc.h"
#include "PubSubClient.h"
/* ref:https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino*/
/* include your librar and add to ini library depe*/

const char* _mqtt_server = "broker.mqtt-dashboard.com";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  wsTextPrintf("Message arrived [");
  wsTextPrintf(topic);
  wsTextPrintf("] ");
  for (int i = 0; i < length; i++) {
    wsTextPrintf("%c",(char)payload[i]);
  }
  wsTextPrintf("\n");

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    wsTextPrintf("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      wsTextPrintf("connected\n");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      wsTextPrintf("failed, rc=");
      wsTextPrintf("%d",client.state());
      wsTextPrintf(" try again in 5 seconds\n");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void cmd_mqtt_setup(){
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  // mqtt client
   randomSeed(micros());
  client.setServer(_mqtt_server, 1883);
  client.setCallback(callback);
}
int cmd_mqtt(int argc,char *argv[]){
    cmd_mqtt_setup();
    while(!wsSerial.escape()){ // loop
        if (!client.connected()) {
            reconnect();
        }
        client.loop();

        unsigned long now = millis();
        if (now - lastMsg > 2000) {
            lastMsg = now;
            ++value;
            snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
            wsTextPrintf("Publish message: ");
            wsTextPrintf(msg);
            wsTextPrintf("\n");
            client.publish("outTopic", msg);
        }
    }; 
    return 0;
}
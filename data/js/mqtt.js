
// Arduino define
var LED_BUILTIN = 2;
var LOW = 0;
var HIGH= 1;
var OUTPUT = 3;

var _mqtt_server = "broker.mqtt-dashboard.com";
var value = 0;
var lastMsg = 0;
var msg;

var client =  MqttClient;

// JavaScript function that takes a Typed Array as an argument
function processByteArray(byteArray) {
    for (var i = 0; i < byteArray.length; i++) {
        print(byteArray[i]);
    }
}

function callback(topic,payload,len) {
    print("Message arrived [",topic,"]",len);
    processByteArray(payload);

   // Switch on the LED if an 1 was received as first character
   if (payload[0] == '1') {
        digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    }else{
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    }

  
}
// Generate a random four-digit hexadecimal string
function generateRandomHex() {
    // Generate a random integer between 0 and 65535 (0xFFFF in hexadecimal)
    var randomNum = Math.floor(Math.random() * 65536);

    // Convert the random integer to a four-digit hexadecimal string
    var hexString = randomNum.toString(16).toUpperCase().padStart(4, '0');

    return hexString;
}

function reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
      print("Attempting MQTT connection...");
      // Create a random client ID
      let  clientId = "ES32Client-"+ generateRandomHex(); //    clientId += String(Math.random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId)) {
        print("connected\n");
        // Once connected, publish an announcement...
        client.publish("outTopic", "hello world");
        // ... and resubscribe
        client.subscribe("inTopic");
      } else {
        print("failed, rc=",client.state());
        print(" try again in 5 seconds\n");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }

function cmd_mqtt_setup(){
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    // mqtt client
    // Math.random(micros()); TODO no seed function
    client.setServer(_mqtt_server, 1883);
    client.setCallback(callback);
  }
function cmd_mqtt(){
      cmd_mqtt_setup();
      while(!wsSerial.escape()){ // loop
          if (!client.connected()) {
              reconnect();
          }
          client.loop();
  
          let now = millis();
          if (now - lastMsg > 2000) {
              lastMsg = now;
              value +=1;
              msg="hello world #%ld"+value;

              print("Publish message: ",msg);
              client.publish("outTopic", msg);
          }
      }; 
      return 0;
  }

  cmd_mqtt();
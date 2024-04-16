// exit restart bug
// Arduino define
let process = require('process');
let esio = require('esio');
let sio = require('sio');
let client = require('mqtt');
let pin = require('gpio');
let esp = require('esp');


// var _mqtt_server = "broker.mqtt-dashboard.com";
var _mqtt_serverip = "192.168.1.106"; //"18.198.222.5";
var value = 0;
var lastMsgTime = 0;
var msg;


// JavaScript function that takes a Typed Array as an argument
function processByteArray(byteArray) {
    for (var i = 0; i < byteArray.length; i++) {
        console.log(byteArray[i]);
    }
}

function callback(topic,payload,len) {
    console.log(esp.FreeHeap(),"Message arrived [",topic,"]",len);
    processByteArray(payload);

   // Switch on the LED if an 1 was received as first character
   if (payload[0] == '1') {
        pin.digitalWrite(pin.LED_BUILTIN, pin.LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    }else{
        pin.digitalWrite(pin.LED_BUILTIN, pin.HIGH);  // Turn the LED off by making the voltage HIGH
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
    while (!wsSerial.escape()&&!client.connected()) {
      console.log("Attempting MQTT connection...");
      // Create a random client ID
      let  clientId = "ES32Client-"+ generateRandomHex(); //    clientId += String(Math.random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId)) {
        console.log("connected",clientId);
        // Once connected, publish an announcement...
        client.publish("outTopic", "hello world");
        // ... and resubscribe
        client.subscribe("inTopic");
      } else {
          console.log("failed, rc=",client.state());
          console.log(" try again in 5 seconds\n");
          // Wait 5 seconds before retrying
          delay(5000);
      }
    }
  }

function cmd_mqtt_setup(){
    pinMode(pin.LED_BUILTIN, pin.OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    // mqtt client
    // Math.random(micros()); TODO no seed function
    if (process.argv.length > 2 && process.argv[2]) {
      console.log('serverip',process.argv[2]);
      client.setServer(process.argv[2], 1883); // dns fail use ip , server not work
    }else{
      console.log('serverip',_mqtt_serverip);
      client.setServer(_mqtt_serverip, 1883); // dns fail use ip , server not work
    }
    client.setCallback(callback);

  }
function cmd_mqtt(){
      cmd_mqtt_setup();
      while(!sio.escape()){ // loop
          if (!client.connected()) {
               reconnect();
          }
          client.loop();
  
          let now = millis();
          if (now - lastMsgTime > 2000) {
              lastMsgTime = now;
              value +=1;
              msg="hello world #%ld"+value;

              console.log("Publish message: ",msg);
              client.publish("outTopic", msg);
          }
      }; 
      client.setCallback(0); // free callback
      client.disconnect(); // stop to avoid native object call jerry callback cause crash
      console.log('disconnecting...');
      
      return 0;
  }

  let ret = cmd_mqtt();
  console.log(ret);

  console.log("Hit by key testing(esc,ctrl-c,'q','Q')...\n");
  console.log(esio.kbhit());
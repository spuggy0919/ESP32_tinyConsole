let pin = require('gpio');
let sio = require('sio');
// constants won't change. They're used here to set pin numbers:
buttonPin = pin.KEY_BUILTIN;  // the number of the pushbutton pin
ledPin = pin.LED_BUILTIN;    // the number of the LED pin

// variables will change:
var buttonState = 0;  // variable for reading the pushbutton status
function setup() {
    // initialize the LED pin as an output:
    pin.pinMode(ledPin, pin.OUTPUT);
    // initialize the pushbutton pin as an input:
    pin.pinMode(buttonPin, pin.INPUT);
  }
  
function loop() {
  // read the state of the pushbutton value:
  buttonState = pin.digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == pin.HIGH) {
    // turn LED on:
    pin.digitalWrite(ledPin, pin.HIGH);
  } else {
    // turn LED off:
    pin.digitalWrite(ledPin, pin.LOW);
  }
}

setup();
var b = false;
while(!b) {
    loop();
    b=sio.escape();
}
LOW=0;
HIGH=1;
INPUT = 1;
OUTPUT = 3;
LED_BUILTIN = 2;
KEY_BUILTIN = 0;
// constants won't change. They're used here to set pin numbers:
buttonPin = KEY_BUILTIN;  // the number of the pushbutton pin
ledPin = LED_BUILTIN;    // the number of the LED pin

// variables will change:
var buttonState = 0;  // variable for reading the pushbutton status
function kbhit(){ // by esc, control-c, q, Q
  b = false;
  while(!b) b=wsSerial.escape();
  return b;
}
function setup() {
    // initialize the LED pin as an output:
    pinMode(ledPin, OUTPUT);
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);
  }
  
function loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}

setup();
var b = false;
while(!b) {
    loop();
    b=wsSerial.escape();
}
let pin = require('gpio');
console.log("LED_BUILTIN=",pin.LED_BUILTIN);
console.log("HIGH=",pin.HIGH);
console.log("LOW=",pin.LOW);
console.log("OUTPUT=",pin.OUTPUT);
console.log("INPUT=",pin.INPUT);
pin.pinMode(pin.LED_BUILTIN,pin.OUTPUT);
pin.digitalWrite(pin.LED_BUILTIN,pin.LOW);
delay(2000);
pin.digitalWrite(pin.LED_BUILTIN,pin.HIGH);

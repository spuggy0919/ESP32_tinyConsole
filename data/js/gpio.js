let pin = require('gpio');

console.log('HIGH',pin.HIGH);
console.log('LED_BUILDIN',pin.LED_BUILTIN);
console.log('OUTPUT',pin.OUTPUT);
pin.pinMode(pin.LED_BUILTIN,pin.OUTPUT);
pin.digitalWrite(pin.LED_BUILDIN,pin.LOW);
let pin = require('gpio');
let sio = require('sio');
var out=0;
function blinkToggle(){
    out = 1-out;
    pin.digitalWrite(pin.LED_BUILTIN,out);
    console.log((out==1) ? "ON":"OFF");
}

function setup(){
    pin.pinMode(pin.LED_BUILTIN,pin.OUTPUT); 
}
function loop(){
    blinkToggle();
    delay(1000);
}
setup();
var b = false;
while(!b) {
    loop();
    b=sio.escape();
}
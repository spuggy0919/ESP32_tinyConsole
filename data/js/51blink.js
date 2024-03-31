
var out=0;
function blinkToggle(){
    out = 1-out;
    digitalWrite(LED_BUILTIN,out);
    print((out==1) ? "ON":"OFF");
}

function setup(){
    LED_BUILTIN = 2;
    OUTPUT = 3;
    pinMode(LED_BUILTIN,OUTPUT); 
}
function loop(){
    blinkToggle();
    delay(1000);
}
setup();
var b = false;
while(!b) {
    loop();
    b=wsSerial.escape();
}
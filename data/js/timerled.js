
var LED_BUILTIN = 2;
var OUTPUT = 3;
pinMode(LED_BUILTIN,OUTPUT); 
let esp = require('esp');

var out=0;
var cycle =0;
function blinkToggle(){
    out = 1-out;
    cycle++;
    digitalWrite(LED_BUILTIN,out);
    print(millis(),cycle,esp.FreeHeap(),(out==1) ? "ON":"OFF");
}
function blink(n, delaytime){
    let i=0;
    if (n<=0) n=1;
    let HIGH = 1;
    let LOW = 0;
    pinMode(LED_BUILTIN,OUTPUT); 
    while(i<n){ 
        digitalWrite(LED_BUILTIN,HIGH); 
        print("HIGH");
        delay(delaytime);              
        digitalWrite(LED_BUILTIN,LOW);
        print("LOW");
        delay(delaytime);
        i+=1;
    }
    digitalWrite(LED_BUILTIN,HIGH); 
}
var id;
var id1;
var id2;
function cleart(){
    clearInterval(id);
    clearTimeout(id1);
    clearTimeout(id2);
}
function blink5_500(){
    blink(5,500);
}
// blinkToggle();
// blink5_500();
let esio = require('esio');
id1  = setInterval(blinkToggle,500);
print("press special key(esc,ctrl-c,'q','Q')...\n");
print(esio.kbhit(),'\n');
clearInterval(id1);
id2  = setInterval(blinkToggle,1000);
print("press special key(esc,ctrl-c,'q','Q')...\n");
print(esio.kbhit(),'\n');
// clearInterval(id2); test auto free 


// id2  = setTimeout(blink5_500,1000);
// print(kbhit(),'stop id1=',id2);
// clearTimeout(id2);
// blink(5,500);
// id2 = setTimeout(blink5_500,1000);
// print(kbhit(),'stop id1=',id1);
// clearInterval(id1);
// print(kbhit(),'stop id2=',id2);
// clearInterval(id2);


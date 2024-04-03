let sio = require('sio'); // websocket serial io 
let touch = require('touch');
let ctx = require('canvas'); // graphics
let pin = require('gpio'); 
let dht = require('dht');

function colorRGBA(color){
    color = color >>> 0; // Ensure the result is an unsigned integer
    ctx.drawPenColor(color);
}
function drawFRect(x,y,w,h,c) {

    this.colorRGBA(c);
    ctx.drawFRect(x,y,w,h);
}
function drawmenu(){
    let offy = 74;
    let offx = 30;
    drawFRect(0,0,640,480,0x808080FF); // clear scr
    drawFRect(128,96,128,96,0x000000FF); // clear scr
    drawFRect(384,96,128,96,0x0000FFFF); // clear scr
    drawFRect(128,288,128,96,0x00FF00FF); // clear scr
    drawFRect(384,288,128,96,0x00FFFFFF); // clear scr
    ctx.drawSetFontSize(60); 
    ctx.drawSetFontColor(0xFF0000FF); //purple
    offy = 72;
    offx = 32;
    ctx.drawText('開',128+offx,96+offy,0);
    ctx.drawText('關',384+offx,96+offy,0);
    ctx.drawText('閃',128+offx,288+offy,0);
    ctx.drawSetFontSize(30); 
    ctx.drawText('DHT22',384+offx-22,288+offy-12,0);
}
var now = new Date(); 
dht.setup(17);
pin.pinMode(pin.LED_BUILTIN,pin.OUTPUT);
drawmenu();

touch.begin();
var touchAttachnow = touch.attach();
var touchAttachlast = touchAttachnow;
var evt = touch.event();
var k = false; // kbhit
var h = false; // dht
var b = true; // blink
var o = pin.HIGH; // output
var current = millis();
var interval = 1000;
while (!k) {
    touchAttachnow = touch.attach();
    if (!touchAttachlast&&touchAttachnow){ // detach then attach for websocket reconnsect
        drawmenu();
    }
    if (touchAttachnow && evt >=129.0) {
        let x=touch.x();
        let y=touch.y();
        console.log('event=',evt,'(',x,',',y,')');
        evt = 0;
        if (x>128&&x<256&&y>96&&y<192) { //ON
            console.log('開');
            pin.digitalWrite(pin.LED_BUILTIN,pin.HIGH);
            b = false;
            o = pin.HIGH;
            h=false;
        }
        if (x>384&&x<512&&y>96&&y<192) { //ON
            console.log('關');
            pin.digitalWrite(pin.LED_BUILTIN,pin.LOW);
            b = false;
            o = pin.LOW;
            h=false;
        }
        if (x>128&&x<256&&y>288&&y<384) { //Blink toggle
            console.log('閃');
            b = true;
            h=false;
        }
        if (x>384&&x<512&&y>288&&y<384) { //END
            h=true;
            console.log('DHT22');
            b = false;
    
        }
        current = millis();    
    }
    if (b&&(millis()-current)>interval){
        current = millis();
        o = pin.HIGH-o; 
        o = o >>> 0;
        pin.digitalWrite(pin.LED_BUILTIN,o);
    }
    if (h&&(millis()-current)>interval){
        current = millis();
        let te=dht.getTemperature(); te=te.toFixed(2);
        let hu=dht.getHumidity(); hu=hu.toFixed(2);
        let ti=now.getHours()+":"+now.getMinutes();
        console.log("at",ti, "temperature=",te,"humitity=",hu);
    }
    k=sio.escape();
    evt = touch.event();
    touchAttachlast = touchAttachnow; 
}
sio.print("\x1b[8m"); // turnoff graphics canvas esc only esc[2J esc[0m esc[8m 


let ctx = require('canvas');
let sio = require('sio');
let touch = require('touch');
var canvasWidth = 640;
var canvasHeight = 480;
function drawFRect() {
    const x = Math.floor(Math.random() * canvasWidth / 2);
    const y = Math.floor(Math.random() * canvasHeight / 2);
    const w = Math.floor(Math.random() * (canvasWidth-x) ); // Max width is half of canvas width
    const h = Math.floor(Math.random() * (canvasHeight-y )); // Max height is half of ctx height
    colorRGBA();
    ctx.drawFRect(x,y,w,h);
}
// Method to draw a filled circle at a random position and radius
function drawFCircle() {
    const x = Math.floor(Math.random() * canvasWidth);
    const y = Math.floor(Math.random() * canvasHeight);
    const maxRadius = Math.min(Math.min(x, y),Math.min(canvasWidth-x, canvasHeight-y)) ; // Max radius is quarter of canvas size
    const r = Math.floor(Math.random() * maxRadius/2) + 1; // Add 1 to ensure radius is at least 1
    colorRGBA();
    ctx.drawFCircle(x,y,r);    
}
function colorRGBA(){
    r = Math.floor(Math.random()*255);
    g = Math.floor(Math.random()*255);
    b = Math.floor(Math.random()*255);
    a = 255;//Math.floor(Math.random()*128+127);
    color =(r<<24)|(g<<16)|(b<<8)|a;
    color = color >>> 0; // Ensure the result is an unsigned integer
    ctx.drawPenColor(color);
}
function clrscr(color){
    color = color>>>0;
    ctx.drawPenColor(color);
    ctx.drawFRect(0,0,640,480);
}
function kbhit(){ // by esc, control-c, q, Q
    let b = false;
    while(!b) {
        b=sio.escape();
    }
    return b;
}

let waitcnt = 0; 
while(!touch.attach()){
    waitcnt+=1;
    if (sio.escape()) break;
}; // wait for websocket connect
sio.print("\x1b[0m"); // turn off
delay(1000);
sio.flush(); 
//清螢幕
color =(230<<8)|(255); //blue
color = color >>>0;
clrscr(color); //blue
//實心方塊
color =(255<<24)|(255); //red
color = color >>>0;
ctx.drawPenColor(color); //red
ctx.drawFRect(100,100,150,150);

//實心圓
color =(180<<24)|(180<<8)|(255); //pink
color = color >>>0;
ctx.drawPenColor(color); //green
ctx.drawFCircle(400,200,100);

//哈囉世界
// sio.print("\x1b[m"); // turnoff graphics canvas
color =(255<<24)|(255<<16)|(255);
color = color >>>0;
ctx.drawSetFontSize(100); 
ctx.drawSetFontColor(color); //purple
ctx.drawText('哈囉世界！',150,100,0);
color =(255<<16)|(255);
color = color >>>0;
ctx.drawSetFontSize(90); 
ctx.drawSetFontColor(color); 
ctx.drawText('tinyConsole',20,250,0);
color =(255<<16)|(240<<8)|(255);
color = color >>>0;
ctx.drawSetFontColor(color); 
ctx.drawSetFontSize(100); 
ctx.drawText('JerryScipt',30,400,0);
console.log("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nesc/crtl-c/'q' to quit",waitcnt); // turnoff graphics canvas
kbhit();
sio.print("\x1b[8m"); // turnoff graphics canvas esc only esc[2J esc[0m esc[8m 
console.log("\n\n");
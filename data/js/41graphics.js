let ctx = require('canvas');
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

clrscr(256);
for(i=0 ; i <300; i++){
    drawFCircle();
}
clrscr(65535);
for(i=0 ; i <300; i++){
    drawFRect();
}
var x=[];
var y=[];
const pts=16;


for(i=0;i<pts;i++){
    x[i]=Math.floor(Math.cos(2*Math.PI*i/pts)*200+320);
    y[i]=Math.floor(Math.sin(2*Math.PI*i/pts)*200+240);
}
clrscr(-1);
ctx.drawStrokeWidth(3);
for(i=0;i<pts;i++){
    for(j=i+1;j<pts;j++){
        colorRGBA();
        ctx.drawLine(x[i],y[i],x[j],y[j]);
    }
}

//清螢幕
color =(230<<8)|(255);
color = color >>>0;
clrscr(color);


//實心方塊
color =(255<<24)|(255);
color = color >>>0;
ctx.drawPenColor(color); //red
ctx.drawFRect(100,100,150,150);

//實心圓
color =(255<<16)|(255);
color = color >>>0;
ctx.drawPenColor(color); //green
ctx.drawFCircle(200,200,50);

//實心方塊
color =(255<<24)|(255<<8)|(255);
color = color >>>0;
ctx.drawStrokeWidth(5); 
ctx.drawPenColor(color); //yello
ctx.drawFRect(300,100,200,200);
//哈囉世界
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
ctx.drawText('JerryScipt',60,400,0);
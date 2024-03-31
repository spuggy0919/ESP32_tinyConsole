function colorRGBA(color){
    color = color >>> 0; // Ensure the result is an unsigned integer
    Canvas.drawPenColor(color);
}
function drawFRect(x,y,w,h,c) {

    this.colorRGBA(c);
    Canvas.drawFRect(x,y,w,h);
}
function drawmenu(){
    let offy = 74;
    let offx = 30;
    drawFRect(0,0,640,480,0x808080FF); // clear scr
    drawFRect(128,96,128,96,0x000000FF); // clear scr
    drawFRect(384,96,128,96,0x0000FFFF); // clear scr
    drawFRect(128,288,128,96,0x00FF00FF); // clear scr
    drawFRect(384,288,128,96,0x00FFFFFF); // clear scr
    Canvas.drawSetFontSize(60); 
    Canvas.drawSetFontColor(0xFF0000FF); //purple
    offy = 72;
    offx = 32;
    Canvas.drawText('開',128+offx,96+offy,0);
    Canvas.drawText('關',384+offx,96+offy,0);
    Canvas.drawText('閃',128+offx,288+offy,0);
    Canvas.drawSetFontSize(30); 
    Canvas.drawText('DHT22',384+offx-22,288+offy-12,0);
}
var now = new Date(); 
var LOW = 0;
var HIGH= 1;
var LED_BUILTIN = 2;
var OUTPUT = 3;
dht.setup(17);
pinMode(LED_BUILTIN,OUTPUT);
drawmenu();

Touch.begin();
var touchAttachnow = Touch.attach();
var touchAttachlast = touchAttachnow;
var evt = Touch.event();
var k = false;
var h = false;
var b = true;
var o = HIGH;
var current = millis();
var interval = 1000;
while (!k) {
    touchAttachnow = Touch.attach();
    if (!touchAttachlast&&touchAttachnow){ // detach then attach for websocket reconnsect
        drawmenu();
    }
    if (touchAttachnow && evt >=129.0) {
        let x=Touch.x();
        let y=Touch.y();
        print('event=',evt,'(',x,',',y,')');
        evt = 0;
        if (x>128&&x<256&&y>96&&y<192) { //ON
            print('開');
            digitalWrite(LED_BUILTIN,HIGH);
            b = false;
            o = HIGH;
            h=false;
        }
        if (x>384&&x<512&&y>96&&y<192) { //ON
            print('關');
            digitalWrite(LED_BUILTIN,LOW);
            b = false;
            o = LOW;
            h=false;
        }
        if (x>128&&x<256&&y>288&&y<384) { //Blink toggle
            print('閃');
            b = true;
            h=false;
        }
        if (x>384&&x<512&&y>288&&y<384) { //END
            h=true;
            print('DHT22');
            b = false;
    
        }
        current = millis();    
    }
    if (b&&(millis()-current)>interval){
        current = millis();
        o = 1-o; 
        o = o >>> 0;
        digitalWrite(LED_BUILTIN,o);
    }
    if (h&&(millis()-current)>interval){
        current = millis();
        let te=dht.getTemperature(); te=te.toFixed(2);
        let hu=dht.getHumidity(); hu=hu.toFixed(2);
        let ti=now.getHours()+":"+now.getMinutes();
        print("at",ti, "temperature=",te,"humitity=",hu);
    }
    k=wsSerial.escape();
    evt = Touch.event();
    touchAttachlast = touchAttachnow; 
}


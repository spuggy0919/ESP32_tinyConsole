JerryScript is a lightweight Javascript interpreter, it only has language capability,
the IO and library function still need C++ wrapper to bridge.

## Here a auto wrapprt generator

```
%js  jswmgen rectangle rect.cpp[enter]
%cat rect.cpp [enter]
#then select copy and paste to your files
%dl rect.cpp [enter]   #download file
```
## How to build wrap cpp file
[HowTo](../../data/js/readme.md)

When you get the autogen file, you should check,
1. if tab postion wrong, bad news!
2. check each methods, if C using same method, different args, it should be merge into one, by check arg_cnt, or using differnet method name.
3. callback wrapper function, should be created. This generator does not generator.
4. non method property, need to set/get, it is function type.
5. bytes array no idea, how to mapping.
   
#### Where does the bridge come from ?
1. iot.js has its own bridge, he build a lot of iotjs_modules-XXX in C language, these become MCU IO layer API, then iotjs integrate JerryScript and porting. A lot of js objects are defined as IO libraries, i.e. gpio, adc, ble, ...
2. For JerryScript only, we need to created these wrappers, here we select Arduino platform, give a example dht sensor, and my console io.

   If you want more functions, you can integrate iotjs or wrap by your own  as my sample code.

### tinyConsole will provide
1. Arduino basic io functions
1. wsSerial wrapper as Input/Output Stream Object
2. tcgraphics wrapper
3. tcfileio wrapper
4. timer (Date(), setInterval,setTimeout,clearInterval)  **BUG unstable**
5. touch (begin, attach, event, x, y)
6. DHT sensor (dht, setup, getTemperature, getHumidity)
7. others (TODO, maybe SPI WIRE and MQTT)
   
### Arduino object 

  /* Digital I/O */
  JERRYXX_BOOL_CHK(jerryx_register_global("pinMode", js_pin_mode));
  JERRYXX_BOOL_CHK(jerryx_register_global("digitalWrite", js_digital_write));
  JERRYXX_BOOL_CHK(jerryx_register_global("digitalRead", js_digital_read));
  /* Time */
  JERRYXX_BOOL_CHK(jerryx_register_global("delay", js_delay));
  JERRYXX_BOOL_CHK(jerryx_register_global("delayMicroseconds", js_delay_microseconds));
  JERRYXX_BOOL_CHK(jerryx_register_global("micros", js_micros));
  JERRYXX_BOOL_CHK(jerryx_register_global("millis", js_millis));
  /* Math */
  JERRYXX_BOOL_CHK(jerryx_register_global("constrain", js_constrain));
  JERRYXX_BOOL_CHK(jerryx_register_global("map", js_map));
  JERRYXX_BOOL_CHK(jerryx_register_global("sq", js_sq));
  /* 'abs', 'max', 'min', 'pow', 'sqrt' functions are supported via javascript 'Math' module */
  /* Trigonometry */
  /* 'cos', 'sin' and 'tan' functions are supported via javascript 'Math' module */
  /* Random Numbers */
  JERRYXX_BOOL_CHK(jerryx_register_global("randomSeed", js_random_seed));
  JERRYXX_BOOL_CHK(jerryx_register_global("random", js_random));
  /* Bits and Bytes */
  JERRYXX_BOOL_CHK(jerryx_register_global("bit", js_bit));
  JERRYXX_BOOL_CHK(jerryx_register_global("bitClear", js_bit_clear));
  JERRYXX_BOOL_CHK(jerryx_register_global("bitRead", js_bit_read));
  JERRYXX_BOOL_CHK(jerryx_register_global("bitSet", js_bit_set));
  JERRYXX_BOOL_CHK(jerryx_register_global("bitWrite", js_bit_write));
  JERRYXX_BOOL_CHK(jerryx_register_global("highByte", js_high_byte));
  JERRYXX_BOOL_CHK(jerryx_register_global("lowByte", js_low_byte));
  /* Analog I/O */
  JERRYXX_BOOL_CHK(jerryx_register_global("analogRead", js_analog_read));
  JERRYXX_BOOL_CHK(jerryx_register_global("analogWrite", js_analog_write));
  JERRYXX_BOOL_CHK(jerryx_register_global("analogReadResolution", js_analog_read_resolution));
  JERRYXX_BOOL_CHK(jerryx_register_global("analogWriteResolution", js_analog_write_resolution));
  /* Advanced I/O */
  JERRYXX_BOOL_CHK(jerryx_register_global("noTone", js_no_tone));
  JERRYXX_BOOL_CHK(jerryx_register_global("pulseIn", js_pulse_in));
  JERRYXX_BOOL_CHK(jerryx_register_global("pulseInLong", js_pulse_in_long));
  JERRYXX_BOOL_CHK(jerryx_register_global("shiftIn", js_shift_in));
  JERRYXX_BOOL_CHK(jerryx_register_global("shiftOut", js_shift_out));
  JERRYXX_BOOL_CHK(jerryx_register_global("tone", js_tone));
  /* External Interrupts */
  JERRYXX_BOOL_CHK(jerryx_register_global("attachInterrupt", js_attach_interrupt));
  JERRYXX_BOOL_CHK(jerryx_register_global("detachInterrupt", js_detach_interrupt));
  /* Interrupts */
  JERRYXX_BOOL_CHK(jerryx_register_global("interrupts", js_interrupts));
  JERRYXX_BOOL_CHK(jerryx_register_global("noInterrupts", js_no_interrupts));
  /* Characters */
  JERRYXX_BOOL_CHK(jerryx_register_global("isAlpha", js_is_alpha));
  JERRYXX_BOOL_CHK(jerryx_register_global("isAlphaNumeric", js_is_alpha_numeric));
  JERRYXX_BOOL_CHK(jerryx_register_global("isAscii", js_is_ascii));
  JERRYXX_BOOL_CHK(jerryx_register_global("isControl", js_is_control));
  JERRYXX_BOOL_CHK(jerryx_register_global("isDigit", js_is_digit));
  JERRYXX_BOOL_CHK(jerryx_register_global("isGraph", js_is_graph));
  JERRYXX_BOOL_CHK(jerryx_register_global("isHexadecimalDigit", js_is_hexadecimal_digit));
  JERRYXX_BOOL_CHK(jerryx_register_global("isLowerCase", js_is_lower_case));
  JERRYXX_BOOL_CHK(jerryx_register_global("isPrintable", js_is_printable));
  JERRYXX_BOOL_CHK(jerryx_register_global("isPunct", js_is_punct));
  JERRYXX_BOOL_CHK(jerryx_register_global("isSpace", js_is_space));
  JERRYXX_BOOL_CHK(jerryx_register_global("isUpperCase", js_is_upper_case));
  JERRYXX_BOOL_CHK(jerryx_register_global("isWhitespace", js_is_whitespace));

#### time object Date see time.js
```
var now = new Date();  // Create a Date object representing the current date and time
print("Date =",now.getDate());         // Get the day of the month
print("Day  =",now.getDay());          // Get the day of the week
print("Year =",now.getFullYear());     // Get the year
print("Hours=",now.getHours());        // Get the hour
print("Mins =",now.getMinutes());      // Get the minutes
print("Secs =",now.getSeconds());      // Get the seconds
print("Milli=",now.getMilliseconds()); // Get the milliseconds
print("Time =",now.getTime());          // Get the number of milliseconds since the Unix epoch
print("toStr=",now.toString());         // Get a string representation of the Date object
```
#### Serial IO wrap as below, example sio.js
    JERRYX_PROPERTY_FUNCTION ("name", js_wsSerial_classname),
    JERRYX_PROPERTY_FUNCTION ("begin", js_wsSerial_begin),
    JERRYX_PROPERTY_FUNCTION ("flush", js_wsSerial_flush),
    JERRYX_PROPERTY_FUNCTION ("available", js_wsSerial_available),
    JERRYX_PROPERTY_FUNCTION ("availableForWrite", js_wsSerial_availableForWrite),
    JERRYX_PROPERTY_FUNCTION ("read", js_wsSerial_read),
    JERRYX_PROPERTY_FUNCTION ("readline", js_wsSerial_readLine),
    JERRYX_PROPERTY_FUNCTION ("getchar", js_wsSerial_getChar),
    JERRYX_PROPERTY_FUNCTION ("escape", js_wsSerial_escape),
    // JERRYX_PROPERTY_FUNCTION ("writebyte", js_wsSerial_write),
    JERRYX_PROPERTY_FUNCTION ("writestring", js_wsSerial_writestring),
    JERRYX_PROPERTY_FUNCTION ("peek", js_wsSerial_peek),
```
function kbhit(){ // by esc, control-c, q, Q
    let b = false;
    while(!b) b=Input.escape();
    return b;
}
function getc(){ // now wsSerial is sent line base, when enter sent
    c=Input.getchar();
    return c;
}

function getline(){
    s=Input.readline();
    return s;
}

function writeline(s){
    return Input.writestring(s);
}

print("Hit by key testing...\n");
print(kbhit());
print("getline...");
str = getline();
print(str,"\n");
print("writeline...");
writeline("中文ABC測試\n");
```
#### DHT object  see dht.js
    JERRYX_PROPERTY_FUNCTION ("setup", js_dht_init),
    JERRYX_PROPERTY_FUNCTION ("getStatus", js_dht_get_status),
    JERRYX_PROPERTY_FUNCTION ("getTemperature", js_dht_get_temperature),
    JERRYX_PROPERTY_FUNCTION ("getHumidity", js_dht_get_humidity),
```
dht.setup(17);
switch(dht.getStatus()) {
    case 0: // NORMAL;
        print("Status Normal");
        break;
    case 1: // timeout
        print("Status Timeout");
        break;
    case 2: // 
    print("Status Checksum error");
        break;
}
print("temperature=",dht.getTemperature());
print("humitity=",dht.getHumidity());
```
#### graphics wrap
    JERRYX_PROPERTY_FUNCTION ("name", js_graphics_classname),
    JERRYX_PROPERTY_FUNCTION ("drawClearScr", js_graphics_drawclearscr),
    JERRYX_PROPERTY_FUNCTION ("drawRect", js_graphics_drawrect),
    JERRYX_PROPERTY_FUNCTION ("drawFRect", js_graphics_drawfrect),
    JERRYX_PROPERTY_FUNCTION ("drawText", js_graphics_drawtext),
    JERRYX_PROPERTY_FUNCTION ("drawPixel", js_graphics_drawpixel),
    JERRYX_PROPERTY_FUNCTION ("drawHLine", js_graphics_drawhline),
    JERRYX_PROPERTY_FUNCTION ("drawVLine", js_graphics_drawvline),
    JERRYX_PROPERTY_FUNCTION ("drawLine", js_graphics_drawline),
    JERRYX_PROPERTY_FUNCTION ("drawArc", js_graphics_drawarc),
    JERRYX_PROPERTY_FUNCTION ("drawFArc", js_graphics_drawfarc),
    JERRYX_PROPERTY_FUNCTION ("drawCircle", js_graphics_drawcircle),
    JERRYX_PROPERTY_FUNCTION ("drawFCircle", js_graphics_drawfcircle),
    JERRYX_PROPERTY_FUNCTION ("drawStrokeWidth", js_graphics_drawstrokewidth),
    JERRYX_PROPERTY_FUNCTION ("drawPenColor", js_graphics_drawpencolor),
    JERRYX_PROPERTY_FUNCTION ("getImage", js_graphics_getimage),
    JERRYX_PROPERTY_FUNCTION ("putImage", js_graphics_putimage),

    JERRYX_PROPERTY_FUNCTION ("drawSetFontSize", js_graphics_drawsetfontsize),
    JERRYX_PROPERTY_FUNCTION ("drawSetFontColor", js_graphics_drawsetfontcolor),

```
//清螢幕
color =(230<<8)|(255);
color = color >>>0;
Canvas.drawPenColor(color); //blue
Canvas.drawClearScr();

//實心方塊
color =(255<<24)|(255);
color = color >>>0;
Canvas.drawPenColor(color); //red
Canvas.drawFRect(100,100,150,150);

//實心圓
color =(255<<16)|(255);
color = color >>>0;
Canvas.drawPenColor(color); //green
Canvas.drawFCircle(200,200,50);

//實心方塊
color =(255<<24)|(255<<8)|(255);
color = color >>>0;
Canvas.drawStrokeWidth(5); 
Canvas.drawPenColor(color); //yello
Canvas.drawFRect(300,100,200,200);
//哈囉世界
color =(255<<24)|(255<<16)|(255);
color = color >>>0;
Canvas.drawSetFontSize(100); 
Canvas.drawSetFontColor(color); //purple
Canvas.drawText('哈囉世界！',150,100,0);
color =(255<<16)|(240<<8)|(255);
color = color >>>0;
Canvas.drawSetFontColor(color); 
Canvas.drawText('JerryScipt',60,400,0);
```
#### touch wrap
    JERRYX_PROPERTY_FUNCTION ("name", js_touch_classname),
    JERRYX_PROPERTY_FUNCTION ("begin", js_touch_begin),
    JERRYX_PROPERTY_FUNCTION ("attach", js_touch_attach),
    JERRYX_PROPERTY_FUNCTION ("event", js_touch_getevent),
    JERRYX_PROPERTY_FUNCTION ("x", js_touch_getx),
    JERRYX_PROPERTY_FUNCTION ("y", js_touch_gety),
```
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
var b = false;
var o = HIGH;
var current = millis();
const interval = 1000;
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
    k=Input.escape();
    evt = Touch.event();
    touchAttachlast = touchAttachnow; 
}

```
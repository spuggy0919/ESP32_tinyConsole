
// ESP32 pindefined

var  NUM_OUPUT_PINS  = 34;
var  PIN_DAC1        = 25;
var  PIN_DAC2        = 26;

var LOW               = 0;
var  HIGH              = 1;
     //GPIO FUNCTIONS
var  INPUT             = 1;
 // Changed OUTPUT from 0x02 to behave the same as Arduino pinMode(pin,OUTPUT) 
 // where you can read the state of pin even when it is set as OUTPUT
var OUTPUT            = 3 ;
var  PULLUP            = 4;
var  INPUT_PULLUP      = 5;
var  PULLDOWN          = 8;
var INPUT_PULLDOWN    = 9;
var OPEN_DRAIN        = 16;
var OUTPUT_OPEN_DRAIN = 19;
var ANALOG            = 192;

var DISABLED  = 0
var RISING    = 1
var FALLING   = 2
var CHANGE    = 3
var ONLOW     = 4
var ONHIGH    = 5
var ONLOW_WE  = 12
var ONHIGH_WE = 13

var LED_BUILTIN = 2;
var KEY_BUILTIN = 0;
var TX = 1;
var RX = 3;
var SDA = 21;
var SCL = 22;
var SS    = 5;
var MOSI  = 23;
var MISO  = 19;
var SCK   = 18;
var A0 = 36;
var A3 = 39;
var A4 = 32;
var A5 = 33;
var A6 = 34;
var A7 = 35;
var A10 = 4;
var A11 = 0;
var A12 = 2;
var A13 = 15;
var A14 = 13;
var A15 = 12;
var A16 = 14;
var A17 = 27;
var A18 = 25;
var A19 = 26;
var T0 = 4;
var T1 = 0;
var T2 = 2;
var T3 = 15;
var T4 = 13;
var T5 = 12;
var T6 = 14;
var T7 = 27;
var T8 = 33;
var T9 = 32;
var DAC1 = 25;
var DAC2 = 26;

print("LED_BUILTIN=",LED_BUILTIN);
print("HIGH=",HIGH);
print("LOW=",LOW);
print("OUTPUT=",OUTPUT);
print("INPUT=",INPUT);
pinMode(LED_BUILTIN,OUTPUT);
digitWrite(LED_BUILTIN,LOW);
print("Led Off\n");
delay(1000);
digitWrite(LED_BUILTIN,HIGH);
print("Led On\n");


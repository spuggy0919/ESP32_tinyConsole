
// ESP32 pindefined

const  NUM_OUPUT_PINS  = 34;
const  PIN_DAC1        = 25;
const  PIN_DAC2        = 26;

const LOW               = 0;
const  HIGH              = 1;
     //GPIO FUNCTIONS
const  INPUT             = 1;
 // Changed OUTPUT from 0x02 to behave the same as Arduino pinMode(pin,OUTPUT) 
 // where you can read the state of pin even when it is set as OUTPUT
const OUTPUT            = 3 ;
const  PULLUP            = 4;
const  INPUT_PULLUP      = 5;
const  PULLDOWN          = 8;
const INPUT_PULLDOWN    = 9;
const OPEN_DRAIN        = 16;
const OUTPUT_OPEN_DRAIN = 19;
const ANALOG            = 192;

const DISABLED  = 0
const RISING    = 1
const FALLING   = 2
const CHANGE    = 3
const ONLOW     = 4
const ONHIGH    = 5
const ONLOW_WE  = 12
const ONHIGH_WE = 13

const LED_BUILTIN = 2;
const KEY_BUILTIN = 0;
const TX = 1;
const RX = 3;
const SDA = 21;
const SCL = 22;
const SS    = 5;
const MOSI  = 23;
const MISO  = 19;
const SCK   = 18;
const A0 = 36;
const A3 = 39;
const A4 = 32;
const A5 = 33;
const A6 = 34;
const A7 = 35;
const A10 = 4;
const A11 = 0;
const A12 = 2;
const A13 = 15;
const A14 = 13;
const A15 = 12;
const A16 = 14;
const A17 = 27;
const A18 = 25;
const A19 = 26;
const T0 = 4;
const T1 = 0;
const T2 = 2;
const T3 = 15;
const T4 = 13;
const T5 = 12;
const T6 = 14;
const T7 = 27;
const T8 = 33;
const T9 = 32;
const DAC1 = 25;
const DAC2 = 26;

console.log("LED_BUILTIN=",LED_BUILTIN);
console.log("HIGH=",HIGH);
console.log("LOW=",LOW);
console.log("OUTPUT=",OUTPUT);
console.log("INPUT=",INPUT);
pinMode(LED_BUILTIN,OUTPUT);
digitalWrite(LED_BUILTIN,LOW);
console.log("Led Off\n");
delay(1000);
digitalWrite(LED_BUILTIN,HIGH);
console.log("Led On\n");


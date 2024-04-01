
// ESP32 pindefined

const  NUM_OUPUT_PINS     = 34;
const  PIN_DAC1           = 25;
const  PIN_DAC2           = 26;

const  LOW                = 0;
const  HIGH               = 1;
     //GPIO FUNCTIONS
const  INPUT              = 1;
 // Changed OUTPUT from 0x02 to behave the same as Arduino pinMode(pin,OUTPUT) 
 // where you can read the state of pin even when it is set as OUTPUT
const OUTPUT              = 3 ;
const  PULLUP             = 4;
const  INPUT_PULLUP       = 5;
const  PULLDOWN           = 8;
const INPUT_PULLDOWN      = 9;
const OPEN_DRAIN          = 16;
const OUTPUT_OPEN_DRAIN   = 19;
const ANALOG              = 192;

const DISABLED            = 0
const RISING              = 1
const FALLING             = 2
const CHANGE              = 3
const ONLOW               = 4
const ONHIGH              = 5
const ONLOW_WE            = 12
const ONHIGH_WE           = 13

const LED_BUILTIN         = 2;
const KEY_BUILTIN         = 0;
const TX                  = 1;
const RX                  = 3;
const SDA                 = 21;
const SCL                 = 22;
const SS                  = 5;
const MOSI                = 23;
const MISO                = 19;
const SCK                 = 18;
const A0                  = 36;
const A3                  = 39;
const A4                  = 32;
const A5                  = 33;
const A6                  = 34;
const A7                  = 35;
const A10                 = 4;
const A11                 = 0;
const A12                 = 2;
const A13                 = 15;
const A14                 = 13;
const A15                 = 12;
const A16                 = 14;
const A17                 = 27;
const A18                 = 25;
const A19                 = 26;
const T0                  = 4;
const T1                  = 0;
const T2                  = 2;
const T3                  = 15;
const T4                  = 13;
const T5                  = 12;
const T6                  = 14;
const T7                  = 27;
const T8                  = 33;
const T9                  = 32;
const DAC1                = 25;
const DAC2                = 26;

if (typeof module !== 'undefined') {  // run examples
    module.exports ={
        NUM_OUPUT_PINS   :NUM_OUPUT_PINS      ,
        PIN_DAC1         :PIN_DAC1            ,
        PIN_DAC2         :PIN_DAC2            ,
        LOW              :LOW                 ,
        HIGH             :HIGH                ,
        INPUT            :INPUT               ,
        OUTPUT           :OUTPUT              ,
        PULLUP           :PULLUP              ,
        INPUT_PULLUP     :INPUT_PULLUP        ,
        PULLDOWN         :PULLDOWN            ,
        INPUT_PULLDOWN   :INPUT_PULLDOWN      ,
        OPEN_DRAIN       :OPEN_DRAIN          ,
        OUTPUT_OPEN_DRAIN:OUTPUT_OPEN_DRAIN   ,
        ANALOG           :ANALOG              ,
        DISABLED         :DISABLED            ,
        RISING           :RISING              ,
        FALLING          :FALLING             ,
        CHANGE           :CHANGE              ,
        ONLOW            :ONLOW               ,
        ONHIGH           :ONHIGH              ,
        ONLOW_WE         :ONLOW_WE            ,
        ONHIGH_WE        :ONHIGH_WE           ,
        LED_BUILTIN      :LED_BUILTIN         ,
        KEY_BUILTIN      :KEY_BUILTIN         ,
        TX               :TX                  ,
        RX               :RX                  ,
        SDA              :SDA                 ,
        SCL              :SCL                 ,
        SS               :SS                  ,
        MOSI             :MOSI                ,
        MISO             :MISO                ,
        SCK              :SCK                 ,
        A0               :A0                  ,
        A3               :A3                  ,
        A4               :A4                  ,
        A5               :A5                  ,
        A6               :A6                  ,
        A7               :A7                  ,
        A10              :A10                 ,
        A11              :A11                 ,
        A12              :A12                 ,
        A13              :A13                 ,
        A14              :A14                 ,
        A15              :A15                 ,
        A16              :A16                 ,
        A17              :A17                 ,
        A18              :A18                 ,
        A19              :A19                 ,
        T0               :T0                  ,
        T1               :T1                  ,
        T2               :T2                  ,
        T3               :T3                  ,
        T4               :T4                  ,
        T5               :T5                  ,
        T6               :T6                  ,
        T7               :T7                  ,
        T8               :T8                  ,
        T9               :T9                  ,
        DAC1             :DAC1                ,
        DAC2             :DAC2                ,
        pinMode :       this.pinMode,
        digitalWrite :  this.digitalWrite,
        digitalRead :   this.digitalRead,
    }
}else{
    console.log('gpio');
    console.log('define const HIGH,LOW,LED_BUILTIN,INPUT,OUTPUT,...');
    console.log('see gpio.js');
}
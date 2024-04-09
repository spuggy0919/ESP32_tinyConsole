### Jerryscript js examples
support require statement for module, the module.js must at directory /js/modules
testng directory still has some problem.
```
0date.js         748         #time
0helloc.js       159         #hello
1sio.js          650         #sio
2fsio.js         746         #fs
3touch.js       3014         #touch
40color.js      1437         #canvas
41graphics.js   2613         #canvas
42graphics.js   1993         #canvas
50d0.js           33         #gpio d0
50d1.js          112         #gpio d1
50pin.js         341         # pin def
50pins.js       1645         # pin def
51blink.js       375         # blink
52button.js     1056         # button test
70dht.js         389         # dht22
blinkt.js        407         # blink
gpio.js          219         # gpio
modules          320         # modules directory 
timer1.js        854         # setInterval
```
## Here a auto wrapper generator
### Fill the methods fields in jswmgen.js
```
methods = {
    classObj : 'rectangle', //  write your own object name
    constructorPrototype : 'Rectangle::Rectangle(int length,int width);',  // should be CLASSNAME::CONSTRUCTOR
    prototypesArray : [// TODO will use only C Prototype , suppose JS is auto generated
    // C properties & methods define here                   
    'int getSize();',
    'int getLength();',
    'int getWidth();'
    ],
    className :'',
    methodNames:[],
}
```
```
%js  jswmgen [enter]
#then select copy and paste to your files
```
When you get the autogen file, you should check,
1. if tab postion wrong, bad news!
2. check each methods, if C using same method, different args, it should be merge into one, by check arg_cnt, or using differnet method name.
3. callback wrapper function, should be created. This generator does not generate.
4. non method property, need to set/get, it is function type.
5. bytes array no idea, how to mapping.

## After review code
1. put the register prototype into  /lib/jswrapper/jswrap_tc.h
```
#ifdef _LIB_RECTANGLE_
bool js_rectangle_classobj_wraper();
#endif
```
1. put the define to include the source file into compiler into /src/cmdconfig.h
```
#define _LIB_RECTANGLE_
```
1. call register prototype into /lib/jswrapper/jswrap_tc_module.cpp, then the rectangle object will load when js vm start up.
```
void jerryxx_register_arduino_library(){

#ifdef _LIB_RECTANGLE_
   js_rectangle_classobj_wraper();
#endif

}
```
2. check your Object is registered.
```
%js dumpg 
```
3. use your Object in js
```
%js rectangle 
```
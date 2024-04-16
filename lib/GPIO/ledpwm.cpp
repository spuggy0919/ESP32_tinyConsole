
/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */

#include "ledpwm.h" 

int PWM_FREQUENCY = 1000; 
int PWM_CHANNEL = 0; 
int PWM_RESOUTION = 8; 
int dutyCycle = 0; 

static int statemachine = -1; // -1:setup , >=0 for loop state

void pwminit() {
if (statemachine !=-1 ) return;
#ifdef ESP32
  Serial.printf("PWM init...done!\r\n");
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOUTION);
  ledcAttachPin(LED_BUILTIN, PWM_CHANNEL);
#else
    // Set GPIO 2 as an OUTPUT
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
#endif
  statemachine = 999; // stoping 
}
bool pwmledChange = true;
String pwmledState = "ON";

void pwmled(int dutycycle){
  // pwmledChange = (dutyCycle == 0) && (dutycycle !=0);
  // pwmledChange = pwmledChange || ((dutyCycle != 0) && (dutycycle ==0));
  pwmledChange = (dutyCycle != dutycycle );
  pwmledState =  (dutycycle<64) ? "OFF" : "ON";
  dutyCycle =  dutycycle;
#ifdef ESP32
  // Serial.printf("PWM duty %d!\r\n",dutyCycle);
  ledcWrite(PWM_CHANNEL, dutyCycle);
#else
 // analogWrite(ledPin, dutycycle);
     digitalWrite(LED_BUILTIN, (dutyCycle==0) ? HIGH:LOW);

#endif
  //delay(100);
}

int pwmledr(){
   return dutyCycle;
}
static int _number= -1;
// static int _Interval= 0;
static unsigned long startTime;
int  pwmtriggernum(){
  return _number;
}
int pwmtriggerstart(){
    statemachine = 0;
    return true;    
}
int pwmtriggerstop(){
    statemachine = 999;
    return true;    
}
bool pwmtriggerStopOrNot(){
  return (statemachine==999);
}
static String pwmlog = "0, 10, 5, 2, 20, 30, 45";
String pwmlogData(){  
   return pwmlog;
}
bool pwmtrigger(int interval, int number, bool randflag){
int temp;
   switch(statemachine){
   case -1: // init GPIO
      pwminit(); // after init entering state = 0 loop state
      statemachine = 999;
      break;
   case 0: // init
     _number = number;
     startTime = millis();
     temp = (randflag) ? random(0,255) : 0;
     pwmled(temp); 
     pwmlog = String(temp);
     statemachine = 1;
     break;
   case 1: // action 
     if (millis()   > (unsigned long)interval + startTime ) {
        startTime = millis();
        temp = (randflag) ? random(0,255) : 255-pwmledr();
        pwmled(temp);
        pwmlog = pwmlog + ", " + String(temp);

        _number = _number - 1;
        if (_number <= 0) {
           statemachine = 999;
           Serial.println(pwmlogData());
          _number = 0;
        }
      }
      break;
   default: 
   //   delay(100);
      break;
   }
  
   return (_number!=0);
}

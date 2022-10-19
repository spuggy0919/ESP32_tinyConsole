/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "keyin.h"

// Button Debounce Read
static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
static unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
static int state = -1;
static bool KeyPressChange = false;
bool KeygetChange(){
   bool r=KeyPressChange;
   KeyPressChange = false;
   return r;
}
bool Keypress(){
    return(state==1);
}
bool Keydepress(){
      return(state==2);
}
int debounceKeyRead(int buttonPin){
  int reading = HIGH;
  int buttonState = HIGH;
    // read the state of the switch into a local variable:
  if (state == -1 ) {
    // setup State 
    pinMode(BUTTON_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode
    
    state = 0; 
    return buttonState;
  }

  // loop 
  reading = digitalRead(buttonPin);
  buttonState = HIGH;

  switch (state){
  
  case 0:  // check pressed
     if (reading == LOW) {
      // reset the debouncing timer
      lastDebounceTime = millis();
      state = 1;
     }
     break;
  case 1: // check duration 
      if (reading == LOW && ((millis() - lastDebounceTime) > debounceDelay)) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
      // if the button state has changed:
          buttonState = LOW;
          lastDebounceTime = millis();
          KeyPressChange = true;
          state = 2;
          return buttonState;
      } 
  case 2:
      if (reading == HIGH && ((millis() - lastDebounceTime) > debounceDelay)) {
         buttonState = HIGH;
         KeyPressChange = true;

         state = 0; // reset too      
      }
      break;
      
  }
 // save the reading. Next time through the loop, it'll be the lastButtonState:
  return HIGH;  
}
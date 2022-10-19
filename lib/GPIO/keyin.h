#ifndef __KEYIN_H__ 
#define __KEYIN_H__ 
#include <Arduino.h>
// constants won't change for ESP32 and ESP8266
#define BUTTON_PIN 0  // Arduino pin connected to button's pin
bool Keypress();
bool Keydepress();
bool KeygetChange();

int debounceKeyRead(int buttonPin); // with Debounce check
#endif // __KEYIN_H__ 
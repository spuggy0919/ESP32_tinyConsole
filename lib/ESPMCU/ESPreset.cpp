/*
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "ESPreset.h"
#ifdef ESP32
#define WDT_TIMEOUT   3                                           // define a 3 seconds WDT (Watch Dog Timer)
int BootReason        = 99;


static int cur_millis;

void ESPHW_config_wdt(){
Serial.println("Configuring WatchDogTimeout WDT...");
    esp_task_wdt_init(WDT_TIMEOUT, true);             // enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);                           // add current thread to WDT watch
    cur_millis = millis();
}

void ESP_force_wdt(int minisec){
        if ( millis() <  minisec + cur_millis  ){
        esp_task_wdt_reset();   // Added to repeatedly reset the Watch Dog Timer      
        }
}
bool ESPHW_powerloss(){
    BootReason = esp_reset_reason();
    if ( BootReason == 1 ) {                          // Reset due to power-on event.
          Serial.println("Reboot was because of Power-On!!");
          return true;
          }
          
    if ( BootReason == 6 ) {                          // Reset due to task watchdog.
          Serial.println("Reboot was because of WDT!!");
          }

    Serial.print("Reset/Boot Reason was: "); Serial.println( BootReason );
    return false;
}
#endif
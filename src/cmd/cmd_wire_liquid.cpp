#include "ESP32inc.h"
#include "command.h"
#ifdef CMD_WIRE
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x3f for a 16 chars and 2 line display
#define I2C_DEV_ADDR 0x3F // lcd 2x16

LiquidCrystal_I2C lcd(I2C_DEV_ADDR, 16, 2);

void cmd_wire_liquid_setup() {
	// initialize the LCD
	lcd.begin(16,2);

	// Turn on the blacklight and print a message.
//   Wire.setPins(SDA,SCL); // SDA SCL
  lcd.init();                      // initialize the lcd 
//   Wire.setPins(SDA,SCL); // SDA SCL
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0); // col, row
  lcd.print(" Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("tinyConsole!");
}

int cmd_wire_liquid(int argc,char *argv[]){
  byte error, address;
  int nDevices = 0;
    cmd_wire_liquid_setup();

    while(!wsSerial.escape()){ // loop

    }
    return 0;
}
#endif
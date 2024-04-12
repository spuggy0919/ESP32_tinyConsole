
let esio = require('esio');
function lcdcls(){
    lcd.setCursor(0,0); // col, row
    lcd.print('                 ');
    lcd.setCursor(0,1); // col, row
    lcd.print('                 ');
}
// should register LiquidCrystal_I2C with jswrapper
// pins SDA SCL and Vcc 5V
const I2C_DEV_ADDR=0x3F; // 0x3f for lcd 2x16 0x3c for oled
const lcd = new LiquidCrystal_I2C(I2C_DEV_ADDR, 16, 2);

lcd.begin(16,2);

lcd.init(); // initialize the lcd 
lcd.backlight();
lcd.setBacklight(128);
lcd.setCursor(1,0); // col, row
lcd.print(" Hello, world!");
lcd.setCursor(2,1);
lcd.print("tinyConsole!");
print('hit key to continous');
esio.kbhit();
var now = new Date();  // Create a Date object representing the current date and time
lcdcls();
let datetime=now.toString();
lcd.setCursor(3,0); // col, row
lcd.print(datetime.slice(4,15));
lcd.setCursor(4,1); // col, row
lcd.print(datetime.slice(16,24));
print('hit key to continous');
esio.kbhit();

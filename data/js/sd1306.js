let esio = require('esio');
const  OLED_RESET = 4;
const  SSD1306_WHITE = 1;
const  SSD1306_BLACK = 0;
const  SSD1306_INVERSE = 2;
const SSD1306_SWITCHCAPVCC = 2;
let  display = new Adafruit_SSD1306(OLED_RESET);   
let logo = [
  0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
];
const 現 =[	//現
0x00,0x08,0x01,0xFC,0xFD,0x08,0x11,0x08,0x11,0xF8,0x11,0x08,0x11,0x08,0x7D,0xF8,
0x11,0x08,0x11,0x08,0x11,0xF8,0x1D,0x48,0xF0,0xA0,0x41,0x22,0x02,0x22,0x0C,0x1E
];
const 在=[	//在
0x02,0x00,0x02,0x00,0x02,0x04,0xFF,0xFE,0x04,0x00,0x04,0x40,0x08,0x40,0x08,0x48,
0x13,0xFC,0x30,0x40,0x50,0x40,0x90,0x40,0x10,0x40,0x10,0x44,0x17,0xFE,0x10,0x00
];
const 時=[	//時
0x00,0x20,0x04,0x20,0x7E,0xF8,0x44,0x20,0x44,0x20,0x45,0xFC,0x44,0x10,0x7C,0x10,
0x47,0xFE,0x44,0x10,0x44,0x90,0x44,0x50,0x7C,0x50,0x44,0x10,0x00,0x50,0x00,0x20
];
const 間=[		//間
0x00,0x08,0x7C,0xFC,0x44,0x88,0x7C,0xF8,0x44,0x88,0x7C,0xF8,0x40,0x08,0x4F,0xC8,
0x48,0x48,0x4F,0xC8,0x48,0x48,0x4F,0xC8,0x48,0x48,0x40,0x08,0x40,0x28,0x40,0x10
];
const logo_bmp = createlogbmp('在');

const LOGO_HEIGHT  = 16;
const LOGO_WIDTH  =  16;
sd1306_setup();
// sd1306_loop();
  // Define the binary data to be assigned to the buffer


function createlogbmp(key){
  // Create an ArrayBuffer with a length of 32 bytes
let buffer = new ArrayBuffer(32);

// Create a typed array view (Uint8Array) to manipulate the buffer
let view = new Uint8Array(buffer);



  // Assign the binary data to the buffer using the view
  // let bmpname = [binaryData, 現,在,時,間];
  let data = key;
  for (let i = 0; i < data.length; i++) {
    view[i] = data[i];
  }

// Now the buffer contains the specified binary data
// console.log(view);
    return buffer;
}
function sd1306_setup(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  
    // Clear the buffer.
    display.clearDisplay();


  // Draw a single pixel in white
  display.drawPixel(10, 20, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  testdrawbitmap();    // Draw a small bitmap image

  testdrawline();      // Draw many lines

  testdrawrect();      // Draw rectangles (outlines)

  testfillrect();      // Draw rectangles (filled)

  testdrawcircle();    // Draw circles (outlines)

  testfillcircle();    // Draw circles (filled)

  testdrawroundrect(); // Draw rounded rectangles (outlines)

  testfillroundrect(); // Draw rounded rectangles (filled)

  testdrawtriangle();  // Draw triangles (outlines)

  testfilltriangle();  // Draw triangles (filled)

  testdrawchar();      // Draw characters of the default font

  testdrawstyles();    // Draw 'stylized' characters

  testscrolltext();    // Draw scrolling text

  testdrawbitmap();    // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

function sd1306_loop(){
    while(!esio.escape()) {
        display.setTextColor(SSD1306_WHITE);                           //設定顯示文字的顏色
        display.setTextSize(1);                                        //設定第1行文字的大小為1
        display.setCursor(0,0);                                        //設定第1行文字在oled上顯示的座標
        display.println("Hello, world!");                         //設定第1行顯示的文字
      
        display.setTextSize(2);                                        //設定第2行文字的大小為2
        display.setCursor(0,15);                                      //設定第2行文字在oled上顯示的座標
        display.println("First");                                       //設定第2行顯示的文字
      
        display.setTextSize(3);                                        //設定第3行文字的大小為3
        display.setCursor(0,8);                                      //設定第3行文字在oled上顯示的座標
        display.println("Second");                                  //設定第3行顯示的文字
      
        display.display();  
        display.startscrollright(0,60);                                               //顯示資料緩衝區內的資料
        delay(10000);
        display.stopscroll();
        display.clearDisplay();                                        //清除資料緩衝區內的資料        
    }
}

function testdrawline() {
    var i;
  
    display.clearDisplay(); // Clear display buffer
  
    for(i=0; i<display.width(); i+=4) {
      display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
      display.display(); // Update screen with each newly-drawn line
      delay(1);
    }
    for(i=0; i<display.height(); i+=4) {
      display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
      display.display();
      delay(1);
    }
    delay(250);
  
    display.clearDisplay();
  
    for(i=0; i<display.width(); i+=4) {
      display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
      display.display();
      delay(1);
    }
    for(i=display.height()-1; i>=0; i-=4) {
      display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
      display.display();
      delay(1);
    }
    delay(250);
  
    display.clearDisplay();
  
    for(i=display.width()-1; i>=0; i-=4) {
      display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
      display.display();
      delay(1);
    }
    for(i=display.height()-1; i>=0; i-=4) {
      display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
      display.display();
      delay(1);
    }
    delay(250);
  
    display.clearDisplay();
  
    for(i=0; i<display.height(); i+=4) {
      display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
      display.display();
      delay(1);
    }
    for(i=0; i<display.width(); i+=4) {
      display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
      display.display();
      delay(1);
    }
  
    delay(2000); // Pause for 2 seconds
  }
  
  function testdrawrect() {
    display.clearDisplay();
  
    for(let i=0; i<display.height()/2; i+=2) {
      display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
      display.display(); // Update screen with each newly-drawn rectangle
      delay(1);
    }
  
    delay(2000);
  }
  
  function testfillrect() {
    display.clearDisplay();
  
    for(let i=0; i<display.height()/2; i+=3) {
      // The INVERSE color is used so rectangles alternate white/black
      display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
      display.display(); // Update screen with each newly-drawn rectangle
      delay(1);
    }
  
    delay(2000);
  }
  
  function testdrawcircle() {
    display.clearDisplay();
  
    for(let i=0; i<Math.min(display.width(),display.height())/2; i+=2) {
      display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
      display.display();
      delay(1);
    }
  
    delay(2000);
  }
  
  function testfillcircle() {
    display.clearDisplay();
  
    for(let i=Math.min(display.width(),display.height())/2; i>0; i-=3) {
      // The INVERSE color is used so circles alternate white/black
      display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
      display.display(); // Update screen with each newly-drawn circle
      delay(1);
    }
  
    delay(2000);
  }
  
  function testdrawroundrect() {
    display.clearDisplay();
  
    for(let i=0; i<display.height()/2-2; i+=2) {
      display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
        display.height()/4, SSD1306_WHITE);
      display.display();
      delay(1);
    }
  
    delay(2000);
  }
  
  function testfillroundrect() {
    display.clearDisplay();
  
    for(let i=0; i<display.height()/2-2; i+=2) {
      // The INVERSE color is used so round-rects alternate white/black
      display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
        display.height()/4, SSD1306_INVERSE);
      display.display();
      delay(1);
    }
  
    delay(2000);
  }
  
  function testdrawtriangle() {
    display.clearDisplay();
  
    for(let i=0; i<Math.max(display.width(),display.height())/2; i+=5) {
      display.drawTriangle(
        display.width()/2  , display.height()/2-i,
        display.width()/2-i, display.height()/2+i,
        display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
      display.display();
      delay(1);
    }
  
    delay(2000);
  }
  
  function testfilltriangle() {
    display.clearDisplay();
  
    for(let i=Math.max(display.width(),display.height())/2; i>0; i-=5) {
      // The INVERSE color is used so triangles alternate white/black
      display.fillTriangle(
        display.width()/2  , display.height()/2-i,
        display.width()/2-i, display.height()/2+i,
        display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
      display.display();
      delay(1);
    }
  
    delay(2000);
  }
  
  function testdrawchar() {
    display.clearDisplay();
  
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE,SSD1306_BLACK); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
    // Not all the characters will fit on the display. This is normal.
    // Library will draw what it can and the rest will be clipped.
    for(let i=0; i<256; i++) {
      if(i == '\n') display.write(' ');
      else          display.write(i);
    }
  
    display.display();
    delay(2000);
  }
  
  function  testdrawstyles() {
    display.clearDisplay();
  
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println("Hello, world!");
  
    display.setTextColor(0, SSD1306_WHITE); // Draw 'inverse' text
    display.println(3.141592);
  
    display.setTextSize(2);             // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    // display.print("0x"); display.println(0xDEADBEEF, HEX); not support
  
    display.display();
    delay(2000);
  }
  
  function testscrolltext() {
    display.clearDisplay();
  
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);
    display.setCursor(10, 0);
    display.println("scroll");
    display.display();      // Show initial text
    delay(100);
  
    // Scroll in various directions, pausing in-between:
    display.startscrollright(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
    display.startscrollleft(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
    display.startscrolldiagright(0x00, 0x07);
    delay(2000);
    display.startscrolldiagleft(0x00, 0x07);
    delay(2000);
    display.stopscroll();
    delay(1000);
  }
  function testdrawbitmap() {
    var key =[現,在,時,間];
    display.clearDisplay();
    let org = 16;
    for(let i =0;i<key.length;i++){
      let 字= createlogbmp(key[i]);

      display.drawBitmap(
        org,
        0,
        字, LOGO_WIDTH, LOGO_HEIGHT, 1,0);
      org+=LOGO_WIDTH+2; 

    }
    display.display();
    var now = new Date();
    display.setTextSize(2);    
    display.setCursor(16,16);
    display.setTextColor(1,0);
    display.println(now.toString().slice(17,25));
    display.display();
    delay(3000);
  }
  
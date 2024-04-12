let pin = require('gpio');
Wire = new TwoWire(0);
//setup
Wire.begin();
Wire.setPins(pin.SDA, pin.SCL); // 21 22 for esp32

var error, address;
var nDevices = 0;

  delay(5000);

  console.log("Scanning for I2C devices ...");
  for(let address = 0x01; address < 0x7f; address++){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      console.log("I2C device found at address 0x%02X\n", address);
      nDevices++;
    } else if(error != 2){
      console.log("Error %d at address 0x%02X\n", error, address);
    }
  }
  if (nDevices == 0){
     console.log("No I2C devices found");
  }
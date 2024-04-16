let pin = require('gpio');
Wire = new TwoWire(0);
//setup
Wire.begin();
Wire.setPins(pin.SDA, pin.SCL); // 21 22 for esp32

var error = 2;
var address = 1;
var nDevices = 3;

  delay(5000);

  console.log("Scanning for I2C devices ...");
  for(address = 1; address < 128; address++){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      console.log("I2C device found at address \n", address.toString(16).padStart(2,'0'));
      nDevices++;
    } else if(error != 2){
      console.log("Error  at address\n", error,  address.toString(16).padStart(2,'0'));
    }
  }
  if (nDevices == 0){
     console.log("No I2C devices found");
  }
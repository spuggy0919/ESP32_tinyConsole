#include "ESP32inc.h"
#include "command.h"
#ifdef CMD_WIRE
#include "Wire.h"

void cmd_wire_setup() {
  Wire.begin();
}

int cmd_wire(int argc,char *argv[]){
  byte error, address;
  int nDevices = 0;
    cmd_wire_setup();

    while(!wsSerial.escape()){ // loop

        delay(5000);

        wsTextPrintf("Scanning for I2C devices ...\n");
        for(address = 0x01; address < 0x7f; address++){
            Wire.beginTransmission(address);
            error = Wire.endTransmission();
            if (error == 0){
                wsTextPrintf("I2C device found at address 0x%02X\n", address);
                nDevices++;
            } else if(error != 2){
                wsTextPrintf("Error %d at address 0x%02X\n", error, address);
            }
        }
        if (nDevices == 0){
            wsTextPrintf("No I2C devices found\n");
        }
    }
    return 0;
}
#endif
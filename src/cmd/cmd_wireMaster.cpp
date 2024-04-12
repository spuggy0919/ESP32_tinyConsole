#include "ESP32Inc.h"
#include "command.h"
#ifdef CMD_WIRE
#include "Wire.h"

#define I2C_DEV_ADDR 0x3F // lcd 2x16

uint32_t i = 0;

void cmd_wireMaster_setup() {
  Wire.begin();
}
void log_print_buf(uint8_t* buf, int length){
    for(int i=0;i<length;i++){
        wsTextPrintf("%c", buf[i]);
    }
    wsTextPrintf("\n");
}

int cmd_wireMaster(int argc,char *argv[]){
  cmd_wireMaster_setup();
  delay(5000);


    while(!wsSerial.escape()){ // loop

        //Write message to the slave
        Wire.beginTransmission(I2C_DEV_ADDR);
        Wire.printf("Hello World! %u", i++);
        uint8_t error = Wire.endTransmission(true);
        wsTextPrintf("endTransmission: %u\n", error);
        
        //Read 16 bytes from the slave
        uint8_t bytesReceived = Wire.requestFrom(I2C_DEV_ADDR, 16);
        wsTextPrintf("requestFrom: %u\n", bytesReceived);
        if((bool)bytesReceived){ //If received more than zero bytes
            uint8_t temp[bytesReceived];
            Wire.readBytes(temp, bytesReceived);
            log_print_buf(temp, bytesReceived);
        }
    }
    return 0;
}
#endif
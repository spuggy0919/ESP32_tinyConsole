/*
 * This file is part of ESP32_TinyConsole.
 *
 * ESP32_TinyConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESP32_TinyConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ESP32_TinyConsole.  If not, see 
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "ESP32inc.h"
/* include your librar and add to ini library depe*/

#include "cmdconfig.h"
#ifdef CMD_DHT
#include "DHTesp.h"
#include "plotLineChart.h"
#include <math.h>
#define DHTPIN 17  // Replace with the pin your sensor is connected to

DHTesp dht;
LineChart chart;
void cmd_dht_setup(){
      dht.setup(DHTPIN,DHTesp::AM2302); // io pin 
}
bool cmd_dht_loop(){
    if (wsSerial.escape()) return false;
    TempAndHumidity data = dht.getTempAndHumidity();

    if (dht.getStatus() != 0) {
         wsTextPrintf("status=%s\n",dht.getStatusString()); // websocket print to console
    
    } else if (isnan(data.humidity) || isnan(data.temperature)) {
         wsTextPrintf("Data is NaN!");

    } else {
         float theta =1.0;
         float y=(sin(2*PI*theta/32.0)*0.8+1.0)/2.0*100.0; theta+=1.0;
         wsTextPrintf("Humidity: "); // websocket print to console
         wsTextPrintf("%f",data.humidity);
         wsTextPrintf("%  Temperature: ");
         wsTextPrintf("%f\n",data.temperature);
         wsMonitorPrintf("humidity=%f\n",data.humidity); // websocket print to monitor
         wsMonitorPrintf("temperature=%f\n",data.temperature); // websocket print to monitor
         chart.insertData(y,data.temperature); //data.humidity,
         chart.plot();
    }

    delay(2000);

    return true;
}



/* don't need to change below, ignore this */
int cmd_dht(int argc,char *argv[]){
int state = 0;
    cmd_dht_setup();
    state=1;
    while(cmd_dht_loop()); 
    return 0;
}

#endif //CMD_DHT
let dht = require('dht');
dht.setup(17);
switch(dht.getStatus()) {
    case 0: // NORMAL;
        console.log("Status Normal");
        break;
    case 1: // timeout
        console.log("Status Timeout");
        break;
    case 2: // 
    console.log("Status Checksum error");
        break;
}
console.log("temperature=",dht.getTemperature());
console.log("humitity=",dht.getHumidity());

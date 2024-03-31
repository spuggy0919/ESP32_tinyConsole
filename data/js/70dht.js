dht.setup(17);
switch(dht.getStatus()) {
    case 0: // NORMAL;
        print("Status Normal");
        break;
    case 1: // timeout
        print("Status Timeout");
        break;
    case 2: // 
    print("Status Checksum error");
        break;
}
print("temperature=",dht.getTemperature());
print("humitity=",dht.getHumidity());

function blink(n, delaytime){
    i=0;
    if (n<=0) n=1;
    LED_BUILTIN = 2;
    OUTPUT = 3;
    HIGH = 1;
    LOW = 0;
    pinMode(LED_BUILTIN,OUTPUT); 
    while(i<n){ 
        digitalWrite(LED_BUILTIN,HIGH); 
        delay(delaytime);              
        digitalWrite(LED_BUILTIN,LOW);
        delay(delaytime);
        i+=1;
    }
    digitalWrite(LED_BUILTIN,HIGH); 
}
blink(5,200);
blink(10,1000);
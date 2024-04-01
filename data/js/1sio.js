let sio = require('sio');
function kbhit(){ // by esc, control-c, q, Q
    b = false;
    while(!b) {
        b=sio.escape();
    }
    return b;
}
function getc(){ // now wsSerial is sent line base, should be enter then sent out
    c=sio.getchar();
    return c;
}

function getline(){
    s=sio.readline();
    return s;
}

function writeline(s){ //wsSerial.write write string to ws tx queue buffer
    return sio.writestring(s);
}

console.log("Hit by key testing(esc,ctrl-c,'q','Q')...\n");
console.log(kbhit());
console.log("getline...");
str = getline();
console.log('\n',str,"\n");
console.log("writeline...");
writeline("中文ABC測試\n");
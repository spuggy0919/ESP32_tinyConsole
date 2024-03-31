function kbhit(){ // by esc, control-c, q, Q
    b = false;
    while(!b) {
        b=wsSerial.escape();
    }
    return b;
}
function getc(){ // now wsSerial is sent line base, should be enter then sent out
    c=wsSerial.getchar();
    return c;
}

function getline(){
    s=wsSerial.readline();
    return s;
}

function writeline(s){ //wsSerial.write write string to ws tx queue buffer
    return wsSerial.writestring(s);
}

print("Hit by key testing(esc,ctrl-c,'q','Q')...\n");
print(kbhit());
print("getline...");
str = getline();
print('\n',str,"\n");
print("writeline...");
writeline("中文ABC測試\n");
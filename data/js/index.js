let fs = require('fs');
let sio = require('sio');
let touch = require('touch');

function kbhit(){ // by esc, control-c, q, Q
    b = false;
    while(!b) {
        b=sio.escape();
    }
    return b;
}

let waitcnt = 0; 
while(!touch.attach()){
    waitcnt+=1;
    if (sio.escape()) break;
}; // if interactive, wait for websocket connect
sio.print("\0x1b[2J");
sio.print("\n\n\n\n\n\n\n\n\n\n");
sio.print("\n\n\n\n\n\n\n\n\n\n");
sio.print("\ntinyConsole\n");
fs.cd('/js');

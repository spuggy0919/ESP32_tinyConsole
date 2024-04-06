let sio = require('sio');
const esio ={
    kbhit:function (){ // by esc, control-c, q, Q
        let b = false;
        while(!b) {
            b=sio.escape();
        }
        return b;
    },
    getc :function(){ // now wsSerial is sent line base, should be enter then sent out
        let c=sio.getchar();
        return c;
    },

    getline : function (){
        let s=sio.readline();
        return s;
    },
 

    writeline : function writeline(s){ //wsSerial.write write string to ws tx queue buffer
        return sio.writestring(s);
    }
}

 
if (typeof module === 'undefined') {
    print("Hit by key testing(esc,ctrl-c,'q','Q')...\n");
    print(esio.kbhit());
    print("getline...");
    str = esio.getline();
    print('\n',str,"\n");
    print("writeline...");
    esio.writeline("中文ABC測試\n");
}
module.exports = esio;
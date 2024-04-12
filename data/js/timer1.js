var id1,id2,id10,iddel;
var t=0;
function f1(){
    print(id1,"1sec Interval",t);
    t+=1;
}
function f2(){
    print(id2,"2sec Interval",t);
}
function f10(){
    print(id10,"10sec Interval",t);
}
function kbhit(){ // by esc, control-c, q, Q
    let b = false;
    while(!b) {
        b=wsSerial.escape();
    }
    return b;
}
// f1(); // jerryscrip bug ? function define after called
// f2();
// f10();

id1 = setInterval(f1,1000);
id2 = setInterval(f2,2000);
id10 = setTimeout(f10,15000);
print(kbhit(),'stop id1=',id1);
clearInterval(id1);
print(kbhit(),'stop id2=',id2);
// clearInterval(id2); // test auto clear by jswrap_tc
print(kbhit(),'stop id3=',id10);
// clearInterval(id10); // auto clear by jswrap_tc
// id2 = setInterval(f2,2000);
// id10 = setTimeout(f10,10000);
// iddel = setTimeout(delt,30000);
function delt(){

    clearInterval(id1);
    clearInterval(id2);
    clearTimeout(id10);
    clearTimeout(iddel);
}
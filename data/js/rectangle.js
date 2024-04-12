/* jsww_rectangle.cpp c++ class  object example */
let esp = require('esp');
let a = [];
for(let i=0;i<100;i++){
    a[i] = new Rectangle(Math.random()*10,Math.random()*40);
    console.log(i,esp.FreeHeap(),a[i].getLength(),'*',a[i].getWidth(),'=',a[i].getSize(),a[i].getSize()==(a[i].getWidth()*a[i].getLength()));
}
for(let i=0;i<100;i++){
  //  delete a[i]; check whether it is auto free
    console.log(i,esp.FreeHeap());
}



/* jsww_rectangle.cpp c++ class  object example */
let a = new Rectangle(30,40);
let b = new Rectangle(3,4);
console.log(a.getLength(),'*',a.getWidth(),'=',a.getSize());
console.log(b.getLength(),'*',b.getWidth(),'=',b.getSize());
delete b;
delete a;


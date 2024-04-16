/* jsww_rectangle.cpp c++ class  object example */
let esp = require('esp');
let rect = new Rectangle(3,4);
console.log('rectangle Length=',rect.getLength(),'width=',rect.getWidth(), 'size=',rect.getSize());
var array = [0xAA, 0x55, 0x12, 0x33];
var tbuf = new  Uint8Array(array);
let abuf = new ArrayBuffer(8);
// Create a DataView to access the buffer
let view = new DataView(abuf);
view.setUint8(0, 0x55);
view.setUint8(1, 0xaa);
view.setUint8(2, 0x34);
view.setUint8(3, 0x12);
view.setUint8(4, 0x4);
view.setUint8(5, 0x5);
view.setUint8(6, 0x6);
view.setUint8(7, 0x7);
rect.getBuffer(tbuf,4); // typearray test
rect.getBuffer(abuf,4); // arraybuffer test
// Create a buffer with 8 bytes (64 bits)
console.log(view);

let typedata = new Uint8Array(abuf);
console.log(tbuf);

console.log(typedata);


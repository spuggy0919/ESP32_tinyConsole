
let esp = new EspClass();
console.log('getHeapSize', esp.getHeapSize());
console.log('getFreeHeap', esp.getFreeHeap());
console.log('getPsramSize', esp.getPsramSize());
console.log('getFreePsram', esp.getFreePsram());
console.log('getFlashChipSize', esp.getFlashChipSize());
console.log('magicFlashChipSize', esp.magicFlashChipSize(2));
console.log('getSketchSize', esp.getSketchSize());
console.log('getSketchMD5', esp.getSketchMD5());
console.log('getFreeSketchSpace', esp.getFreeSketchSpace());
var abuf = new ArrayBuffer(8);
var dataView = new Uint8Array(abuf); 
dataView[0]=0x55;
dataView[1]=0xAA;
dataView[2]=0x12;
dataView[3]=0x34;
dataView[4]=0x45;
dataView[5]=0x67;
dataView[6]=0x89;
dataView[7]=0xAB;

// Log each item in the data view with hex bytes
dataView.forEach(byte => {
    console.log(byte.toString(16).padStart(2, '0')); // Convert byte to hex and log with padding
});
console.log('getEfuseMac', esp.getEfuseMac(abuf));
dataView.forEach(byte =>{
    console.log(byte.toString(16).padStart(2,'0'));
});
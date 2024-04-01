let obj=[this,wsSerial,File,Touch,Canvas.dht,mqtt];
for(let o in obj){
    print(o,o.keys,dumpObject(obj));
}
function dumpObject(obj) {
var result='';
for (let key in obj) {
    if (typeof obj[key] === 'function') {
      // If property is a function, stringify its definition
      result += `${' '.repeat(depth * 2)}${key}: function },\n`;
    } else if (typeof obj[key] === 'object' && obj[key] !== null) {
      // If property is an object, recursively dump it
      result += `${' '.repeat(depth * 2)}${key}: object,\n`;
    } else {
      // For other types, stringify the value
      result += `${' '.repeat(depth * 2)}${key}: property ${JSON.stringify(obj[key])},\n`;
    }
  }
}
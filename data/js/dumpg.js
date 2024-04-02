let nativeobjs=[this,wsSerial,File,Touch,Canvas,dht];
nativeobjs.forEach(element => {
  // print(element);
  listprops(element);
});
function listprops(obj){
  // Enumerate and list all elements (including functions) in the object
  for (let key in obj) {
    if (obj.hasOwnProperty(key)) {
      console.log(key,"\t",obj[key]); // Print the property name
      // console.log(obj[key]); // Print the property value (function in this case)
    }
  }
} 
/* not work below */
function dumpObject(obj) {
var result='';
var depth = 0;
for (let key in obj) {
    if (typeof obj[key] === 'function') {
      // If property is a function, stringify its definition
      result += `${' '.repeat(depth * 2)}${key}: function },\n`;
    } else if (typeof obj[key] === 'object' && obj[key] !== null) {
      // If property is an object, recursively dump it
      result += `${' '.repeat(depth * 2)}${key}: object,\n`;
      dumpObject(obj[key]);
    } else {
      // For other types, stringify the value
      result += `${' '.repeat(depth * 2)}${key}: property ${JSON.stringify(obj[key])},\n`;
    }
  }
}
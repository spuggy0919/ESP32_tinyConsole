let globalobj=this
print(this);


var console = {
    log: function(msg){
        // Convert the 'arguments' object to an array
        var args = Array.prototype.slice.call(arguments);

        // Join the arguments into a single message
        var message = args.join(' ');

        // Log the message
        print(message); // You can replace 'print' with your actual logging logic
    }
}
function customDump(obj) {
    // Create an array to hold the dumped properties and methods
    let dumped = [];
  
    // Helper function to stringify functions with their definitions
    function stringifyFunction(func) {
      let funcString = func.toString();
      return funcString.substring(funcString.indexOf('{') + 1, funcString.lastIndexOf('}')).trim();
    }
  
    // Recursive function to dump properties and methods
    function dumpObject(obj, depth = 0) {
      if (depth > 5) {
        return '[Depth Limit Exceeded]';
      }
  
      let result = '{\n';
      for (let key in obj) {
        if (typeof obj[key] === 'function') {
          // If property is a function, stringify its definition
        //   result += `${' '.repeat(depth * 2)}${key}: function ${stringifyFunction(obj[key])},\n`;
          result += `${' '.repeat(depth * 2)}${key}: function(),\n`;
        } else if (typeof obj[key] === 'object' && obj[key] !== null) {
          // If property is an object, recursively dump it
          result += `${' '.repeat(depth * 2)}${key}: ${dumpObject(obj[key], depth + 1)},\n`;
        } else {
          // For other types, stringify the value
          result += `${' '.repeat(depth * 2)}${key}: ${JSON.stringify(obj[key])},\n`;
        }
      }
      result += ' '.repeat((depth - 1) * 2) + '}';
      return result;
    }
  
    // Dump the object recursively
    dumped.push(dumpObject(obj));
    
    // Join all dumped objects into a single string
    return dumped.join(',\n');
  }
  
  // Example usage:
  let obj = {
    prop1: 'value1',
    prop2: 42,
    method1: function() {
      console.log('This is a method.');
    },
    nestedObj: {
      prop3: 'value3',
      method2: function() {
        console.log('Another method.');
      }
    }
  };
  
  let dump = customDump(obj);
  console.log(dump);
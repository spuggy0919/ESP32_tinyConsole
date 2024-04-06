let process = require('process');
listProps(process);
console.log("argc=",process.argv.length);
let i=0;
process.argv.forEach(function(element){
    console.log(`argv[${i}]`,element);     i++;
});

let fs = require('fs');
fs.mkdir('abc');
console.log('mkdir',fs.ls('abc'));
fs.cd('abc');
console.log(fs.cd());
fs.writeFile('test.js','console.log("Hello World!");\n');
console.log('writeFile','test.js','console.log("Hello World!");\n');
console.log('readFile',fs.readFile('test.js'));
let script = fs.readFile('test.js');
eval(script);
fs.appendFile('test.js','console.log("哈囉歡迎您！");\n');
console.log('appendFile','console.log("哈囉歡迎您！");\n');
console.log('readFile',fs.readFile('test.js'));
let script1 = fs.readFile('test.js');
eval(script1);
fs.writeFile('abd.js','console.log("Hello World!");');
console.log('ls',fs.ls());
console.log('rm','abd.js');
fs.deleteFile('abd.js');
console.log('ls',fs.ls());
fs.cd('..');


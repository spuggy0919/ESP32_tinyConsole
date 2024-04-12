
let fs = require('fs');
fs.mkdir('abc');
console.log('mkdir',fs.ls('abc'));
fs.cd('abc');
console.log('cwd=',fs.cd()); 
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

let fsinfo = fs.df();
// Create an empty dictionary
function parser_dfinfo(inputString){// Loop through each line
    var data = {};
    // Split the string by newline
    var lines = inputString.split('\n');
    for (var i = 0; i < lines.length; i++) {
        var line = lines[i];
        // Find the position of '=' character
        var pos = line.indexOf('=');
        if (pos !== -1) {
            // Extract the keyword before '=' and trim spaces
            var keyword = line.substr(0, pos).trim();

            // Extract the substring after '=' and trim spaces
            var valueStr = line.substr(pos + 1).trim();

            // Convert the substring to an integer
            var number = parseInt(valueStr);

            // Add the keyword and number to the dictionary
            data[keyword] = number;
        }
    }
    return data;
}
// Split the string by newline
let dfobj = parser_dfinfo(fsinfo);
console.log(dfobj.FreeHeap);
// Convert the dictionary to JSON
var jsonData = JSON.stringify(data);

fs.cd('..');


const fs = require('fs');
const process = require('process');

const cwd = process.cwd();
// Read the .h file content
const filePath = cwd+process.argv[2]; // Replace 'example.h' with your file path
// const filePath = cwd + 'esp.h';
const fileContent = fs.readFile(filePath);

if (typeof process.argv[2] === 'undefined' || fileContent == '') {
    console.log(`Input file not Found ${filePath}`);
    process.exit(-1);
}
let methods = parserMethodsObj(fileContent);
if (methods.className == '' || methods.constructorPrototype == '') {
    console.log(`Parser result error ${methods.className}`);
    process.exit(-2);
}
const outputFile = cwd + methods.className + ".json";
methods.prototypesArray.forEach( (element, index, array)=>{
    // let old = element;
    let e = element.replace(/const\s+/g,'');  //remove const
    let f = e.replace(/\s+\*\s*/g,'* '); //type *var ==> type* var
    array[index] = f;
    // console.log(old, f);
});
dumpMethodsObj(methods);

let jsonStr = JSON.stringify(methods);
let estr = jsonStr.replace(/",/g,'",\n');
let fstr = estr.replace(/\["/g,'[\n"');
let lineStr = fstr.replace(/\],/g,'],\n');
fs.writeFile(outputFile,lineStr);

function parserMethodsObj(fileContent){
    let methods = {
        className :"",
        classObj : "rectangle", 
        constructorPrototype : "Rectangle::Rectangle(int length,int width);", 
        prototypesArray : [
        ],
        methodNames:[]    
    };        
    
    // Regular expression patterns to extract class name, constructor prototype, and method prototypes
    const classPattern = /class\s+(\w+)\s*{/;
    // Find the class name
    const classNameMatch = fileContent.match(classPattern);
    const className = classNameMatch ? classNameMatch[1] : null;
    methods.className = className;
    methods.classObj = toLowerCase(className);
    try {
        let constructorPattern =new RegExp(`^\\s*(${className}\\s*\\(.*\\))`,'gm');
        // Find the constructor prototype
        let constructorMatch = fileContent.match(constructorPattern);
        if (constructorMatch) {
            const constructorPrototype = className+'::'+constructorMatch[0].trim();
            methods.constructorPrototype = constructorPrototype;
            console.log(`Constructor Prototype: ${constructorPrototype}`);
        } else {
            console.log('Constructor prototype not found.');
            methods.constructorPrototype = "";
            throw new Error(`Constructor prototype not found.`);
        }
    } catch(e) {
        console.log('Error',e.message);
    }
    // Find method prototypes
    const methodPattern = /(\w+)\s+(\w+)\(.*\);/g;
    const methodPrototypes = fileContent.match(methodPattern) || [];
    methods.prototypesArray = methodPrototypes;
    return methods;

}

function dumpMethodsObj(methods){
    // Print the extracted information
    console.log("Class Obj:", methods.classObj);
    console.log("Class Name:", methods.className);
    console.log("Constructor Prototype:", methods.constructorPrototype);
    console.log("Method Prototypes:");
    methods.prototypesArray.forEach(method => console.log(method.trim()));
}

function toLowerCase(str) {
    var result = '';
    for (var i = 0; i < str.length; i++) {
        var charCode = str.charCodeAt(i);
        if (charCode >= 65 && charCode <= 90) { // ASCII range for uppercase letters
            result += String.fromCharCode(charCode + 32); // Convert uppercase to lowercase
        } else {
            result += str.charAt(i); // Keep non-alphabetic characters unchanged
        }
    }
    return result;
}
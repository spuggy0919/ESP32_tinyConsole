/*
 * This file is part of ESP32_TinyConsole.
 *
 *  Copyright (c) 2024 spuggy0919
 *
 * ESP32_TinyConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESP32_TinyConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ESP32_TinyConsole.  If not, see 
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
/*
 *
 */
/*
 * @file  jswmgen.js
 * @brief C++ class wrapper generator
 * 
 * @details
 *   run this script parser a json file to generate wrapper source code
 *   %js jswmgen [methods.json [output.cpp]]
 
 *   readme to add into * 
 * this generator for method property , check mapping arglist and call obj method
 * TODO callback setting is not generator, need manual dirty work 
 * Byte* array mapping use arraybuffer
 * How to use
 * 1. fill the need field in methods
 *    classObj your objname , for example 'rect'
 *    constructorPrototype :: is needed, for example 'Rectangle::Rectangle(int length,int width);'
 *    prototypesArray
 * 2. upload to tinyconsole
 * 3. js jswmgen [enter]
 * 4. select, copy and paste to file.
 *    Example rectangle
 *    %js jswmgen rectangle.json rect.cpp
 *    %cat rect.cpp       #select copy and past 
 ** @classname None
 * @instance none
 * 
 * @date Apirl 15, 2024
 */

// TODO for command line mode
// TODO usage js jswmgen methos.json file.cpp 
// todo method prototype array using Object.array.forEach to generate all method handles


/* main */
    // type mapping (c_arg_type->js_arg_type) (cret->jsrettype)
    // maybe need to correction

let process = require('process');   
console.log(process.cwd());
const version = '0.9.0';
const app = process.argv[1];
let fs = require('fs');  
let jsonFileName='/js/jswmgen/esp.json';
let outFileName='/js/jswmgen/esp.cpp';
var methods;
var cobj;
var lines = 0;

if (process.argv.length>2) {
    jsonFileName = process.cwd()+process.argv[2];
    if (process.argv.length==4) outFileName=process.cwd()+process.argv[3];
}
const [argMappings_C_to_JS,retMappings_C_to_JS] = definedConst();
[methods,cobj] = loadJsonFile(jsonFileName);
dumpMethods(jsonFileName,outFileName,methods);
pass1CheckMethods(jsonFileName,outFileName,methods);

jswGenerator(methods);
if (outFileName!='') console.log(`Total ${lines} lines`);

/* load method Object which is defined in JSON file */
function loadJsonFile(jsonFile){
let methods={ // default
    classObj : "rectangle", 
    constructorPrototype : "Rectangle::Rectangle(int length,int width);", 
    prototypesArray : [
        "int getSize();",
        "int getLength();",
        "int getWidth();",
        "int getBuffer(uint8_t* buf, size_t length);" // test arraybuffer
    ],
    className :"",
    methodNames:[]    
};
    const configStr = fs.readFile(jsonFile);
    methods = JSON.parse(configStr); 
    if (typeof methods == 'undefined') {
        console.log('Error: JSAON Parser');
    }
    let cobj = constructorPrototypeParser(methods.classObj,methods.constructorPrototype);
    // methods.className = cobj.className;
    methods.methodNames=[];
    return [methods,cobj]; 
}

function dumpMethods(jsonFile,outFile,methods){
    console.log('//Json:',jsonFile);
    console.log('//File:',outFile);
    console.log('//className:',methods.className);
    console.log('//classObj:',methods.classObj);
    console.log('//constructorPrototype:',methods.constructorPrototype);
    console.log('//prototypesArray:');
    methods.prototypesArray.forEach(element => {
        // print(element);
        console.log('//   ',element);
    });
    if (outFile!=''){
        fs.deleteFile(outFile);
        fs.writeFile('// '+outFile,app+version);
        writeln(outFile,'//Json:'+jsonFile);
        writeln(outFile,'//File:'+outputFile);
        writeln(outFile,'//className:'+methods.className);
        writeln(outFile,'//classObj:'+methods.classObj);
        writeln(outFile,'//constructorPrototype:'+methods.constructorPrototype);
        writeln(outFile,'//prototypesArray:');
        methods.prototypesArray.forEach(element => {
            // print(element);
            writeln(outFile,'//   '+element);
        });
    }

}

function pass1CheckMethods(jsonFile,outFile,methods){

    methods.prototypesArray.forEach(element => {
        console.log('pass1CheckMethods:',element);
        methodParserCheckeer(element);
    });
}
function methodParserCheckeer(methodPrototype) {
    let methodElementsC = extractMethodElements(methodPrototype);
    let methodName = methodElementsC.methodName;
    let returnctype = methodElementsC.rettype;
    //TODO retMappings_C_to_JS[returnctype];
    //TODO concerns call method arg typecast

    let returnjstype = retMappings_C_to_JS[returnctype];//methodElementsJS.rettype;
    
    let argvCnt = methodElementsC.argvCnt;

    try {
        if (typeof returnjstype === 'undefined' || returnjstype === ''){
            returnjstype = 'undefined';
            throw new Error(`Exception retType ${returnctype}, js is null`);
        }
        if (argvCnt == 0) return;
        let argvtype = methodElementsC.argvtypeArray;
        let argvname = methodElementsC.argvnameArray;
        argvtype.forEach(element => {
            let argCtype = argMappings_C_to_JS[element]; // print(element);
            if (typeof returnjstype === 'undefined' || argCtype === ''){
                throw new Error(`Exception argType ${methodName} ${argCtype}`);
            }
        });  
        argvname.forEach(element => {
            if (typeof returnjstype === 'undefined' || argvname === ''){
                throw new Error(`Exception argName ${methodName} ${argvname}`);
            }
        });   
    }catch(e){
        console.error('Error:', e.message);        
    }   
}

function outputFile(statements){
    if (outFileName!=''){ // outFileName global
        // append file TODO
        writeln(outFileName,statements);
        return;
    }
    console.log(statements);
}
function outputFileBoth(statements){
    if (outFileName!=''){ // outFileName global
        // append file TODO
        writeln(outFileName,statements);
    }
    if (statements.length>10) console.log(statements," Done!");
}
function writeln(outFile,msg){
    fs.appendFile(outFile,msg);
    fs.appendFile(outFile,"\n");
    lines++;
}
function definedConst(){
    return [definedConstArg(),definedConstRet()];
}
function definedConstArg(){
        const argMappings_C_to_JS = { // TODO for arg_type mappings type 
        "size_t": "uint32",
        "int": "int32",
        "int_t": "int32",
        "int8_t": "int8",
        "int16_t": "int16",
        "int32_t": "int32",
        "uint": "uint32",
        "uint8": "uint8",
        "uint16": "uint16",
        "uint32": "uint32",
        "uint_t": "uint32",
        "uint8_t": "uint8",
        "uint16_t": "uint16",
        "uint32_t": "uint32",
        "uint_t*": "arraybuffer",
        "uint8_t*": "arraybuffer",
        "uint16_t*": "arraybuffer",
        "uint32_t*": "arraybuffer",
        "int_t*": "arraybuffer", // ignore part should be modified as GFX drawbitmap
        "int8_t*": "arraybuffer",
        "int16_t*": "arraybuffer",
        "int32_t*": "arraybuffer",        
        "char*": "string", // possible use string_sz
        "char": "uint8",
        "char*": "string",
        "double": "number",
        "float": "number",
        "bool": "boolean",
        "ignore": "ignore",         // TODO for callback should be typedef funcptr prototype
        "void*": "native_pointer",    // TODO for callback should be typedef funcptr prototype     
        "funcptr": "function",   // TODO for callback should be typedef funcptr prototype
        "custome": "custome"   // TODO for callback should be typedef funcptr prototype
    };
    return argMappings_C_to_JS;
}
function definedConstRet(){
    const retMappings_C_to_JS = { // TODO for return js type 
        "size_t": "number",
        "int8": "number",
        "int16": "number",
        "int32": "number",
        "int": "number",
        "int_t": "number",
        "int8_t": "number",
        "int16_t": "number",
        "int32_t": "number",
        "uint8": "number",
        "uint16": "number",
        "uint32": "number",
        "uint": "number",
        "uint_t": "number",
        "uint8_t": "number",
        "uint16_t": "number",
        "uint32_t": "number",
        "uint8*": "string",
        "uint8_t*": "string",
        "char*": "string",
        "char": "number",
        "bool": "boolean",
        "void": "void"   // undefined is keywordmaybe transfer in functions
    };// TODO crettype auto transfer to jsrettype

    return retMappings_C_to_JS;
}  
function jswGenerator(methods){

    // #ifdef _LIB_CLASS_
    LibdefHeaderGeneratorPush(cobj); 
    // native_info
    destroyinfoGenerator(cobj);
    // Methods
    methodsGenerator(methods);
    //,free_cb
    destroyFreeCBGenerator(cobj);
    outputFile(`//Verify Methods List`);
    cobj.methods = methods.methodNames;
    cobj.methods.forEach((element) => {
       outputFile(`/*js_${cobj.classObjName}_${element}*/`);
    });  
    // constructor
    constructorGenerator(cobj);
    // register
    registerGenerator(cobj);
    LibdefTailGeneratorPop();
}
function methodsGenerator(methods){
    methods.prototypesArray.forEach(( prototypeC ) => {
        let methodName = methodGenerator(methods.className,methods.classObj,prototypeC);
        methods.methodNames.push(methodName);
    });
}
function methodGenerator(className,classObj,prototypeC) {
    let methodElementsC = extractMethodElements(prototypeC);
    let methodName = methodElementsC.methodName;
    let argvtype = methodElementsC.argvtypeArray;
    let argvname = methodElementsC.argvnameArray;
    let returnctype = methodElementsC.rettype;
    //TODO retMappings_C_to_JS[returnctype];
    //TODO concerns call method arg typecast
    let returnjstype = retMappings_C_to_JS[returnctype];//methodElementsJS.rettype;
    if (typeof returnjstype === 'undefined' || returnjstype === ''){
        returnjstype = 'undefined';
        throw new Error(`Exception retType ${returnctype}, js is null`);
    }
    let argvCtype = methodElementsC.argvtypeArray;

    try {
        genFunctionHeaderPush(classObj,prototypeC,methodName);

    //2. mapping argument
            argList(argvtype,argvname);
            mapping(argvtype,argvname);
            transform(argvtype);
            getNativePtr(classObj);
            callClassObjMethod(className,classObj,returnjstype,methodName,argvname,argvtype,argvCtype);

        genFunctionTailPop();
    } catch (e) {
        // Catch and handle the error
        console.error('Error:', e.message);
    }
    return methodName;
}

var blocks=[];
var level=0;
function resetblock(){
    blocks=[];
    level=0;
}
function pushblock(f){
    blocks.push(f);
    level++;
}
function popblock(){
    s = blocks.pop();
    level--;
    outputFileBoth(s);
}
function genFunctionHeaderPush(classObj,prototypeC,methodName){
    outputFile(`//${prototypeC}`); // method prototype
    //1. method prototype
    outputFile(`JS_METHOD(${classObj}_${methodName})`); // method prototype
    outputFile(`{`);
    pushblock(`} /*js_${classObj}_${methodName}*/`);
}
function genFunctionTailPop(){
    popblock();
}
function parserMatch(match) {
    // Extracted elements
    const rettype = match[1].trim();
    const methodName = match[2].trim();
    // console.log(rettype,retMappings_C_to_JS[rettype]);
    // console.log(methodName);    
    // Check if the argument list is empty
    if (match[3] === '') {
        // Handle the case of an empty argument list
        const argCnt = 0;
        const argvtypeArray = [];
        const argvnameArray = [];

        // console.log(argCnt);
        
        // Return the extracted elements as an object
        return {
            rettype,
            methodName,
            argCnt,
            argvtypeArray,
            argvnameArray
        };
    }
    const argList = match[3];

    // Split the argument list into individual arguments
    const argArray = argList.split(',').map(arg => arg.trim());

    // Extract argument types and names
    const argvtypeArray = argArray.map(arg => arg.split(' ')[0].trim());
    const argvnameArray = argArray.map(arg => arg.split(' ')[1].trim());

    // Argument count
    let argCnt = 0;
    if ((typeof argArray !== undefined )) {
        argCnt = argArray.length;
    }
    // Return the extracted elements as an object
    return {
        rettype,
        methodName,
        argCnt,
        argvtypeArray,
        argvnameArray
    };
}
function extractMethodElements(methodPrototype) {
    // Regular expression pattern to match the method prototype syntax
    const pattern = /^(\w+\*?)\s+(\w+)\((.*?)\);?$/;
    // Execute the regular expression on the method prototype
    const match = methodPrototype.match(pattern);
    if (match) {
        return parserMatch(match);
    // } else {
        // Method prototype does not match the expected syntax
        // throw new Error(`Method prototype extract error  ${methodPrototype}`);
        // return null;
    }
    return null;
}

function argList(argvtype,argvname){
    if (typeof argvtype === 'undefined' || argvtype.length == 0){
        
        outputFile(`${' '.repeat(level * 4)}JERRYX_UNUSED(call_info_p);`);
        outputFile(`${' '.repeat(level * 4)}JERRYX_UNUSED(args_p);`);
        outputFile(`${' '.repeat(level * 4)}JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != 0, "Wrong arguments count");`);    
        return 0;  
    }
    outputFile(`${' '.repeat(level * 4)}JERRYX_UNUSED(call_info_p);`);
    //2. mapping argument
    let i=0;
    for(i=0;i<argvtype.length;i++) {
        if (argvtype[i]=='char*'){
            outputFile(`${' '.repeat(level * 4)}char ${argvname[i]}[32];// **WARNNING** need to check BUFFERSIZE`);
        }else{
            outputFile(`${' '.repeat(level * 4)}${argvtype[i]} ${argvname[i]};`);
        }
    } 
    return argvtype.length;   
}
function mappingEntry_string(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_string(${argvname}, sizeof(${argvname}), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_array(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_array(&${argvname},  JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_int(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_${argvtype}(&${argvname}, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_native_pointer(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}/* **TODO** need to define your c callback to use jerry_call */`);
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_function(&${argvname}, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_function(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}/* **TODO** need to define your c callback to use jerry_call */`);
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_function(&${argvname}, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_coerce_required(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_${argvtype}(&${argvname},  JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_coerce_optional(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_${argvtype}(&${argvname},  JERRYX_ARG_NO_COERCE, JERRYX_ARG_OPTIONAL),`);
}
function mappingEntry_arraybuffer(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_${argvtype}(&${argvname},   JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_ignore(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_ignore (void), // *TODO*ignore should be modified as GFX drawbitmap, to get arraybuffer_data`);
}
function mapping(argvtype,argvname){
    if (typeof argvtype=== 'undefined' || argvtype.length == 0) return 0;
    outputFile(`\n${' '.repeat(level * 4)}const jerryx_arg_t mapping[] ={`); lines++; // for newline
    pushblock(`${' '.repeat(level * 4)}};`);
    let i=0;
    for(i=0;i<argvtype.length;i++) {
        let jsargtype= argMappings_C_to_JS[argvtype[i]];
        switch(jsargtype){// convert c type to mapping
        case 'string':
            mappingEntry_string(argvname[i],argvtype[i]);
            break;
        case 'array':
            mappingEntry_array(argvname[i],argvtype[i]);
            break;
        case 'number':
        case 'boolean':
            mappingEntry_coerce_required(argvname[i],jsargtype);
            break;            
        case 'int8':
        case 'int16':
        case 'int32':
        case 'uint8':
        case 'uint16':
        case 'uint32': 
            mappingEntry_int(argvname[i],jsargtype);
            break;
        case 'arraybuffer': 
            mappingEntry_arraybuffer(argvname[i],jsargtype);
            break;
        case 'function': 
            mappingEntry_function(argvname[i],jsargtype);
            break;
        case 'native_point': 
            mappingEntry_native_pointer(argvname[i],jsargtype);
        case 'ignore': 
            mappingEntry_ignore(argvname[i],jsargtype);
            break;
        default:
            console.log('ERROR ',argvtype[i],argvname[i],'for arg Mapping()');
            throw new Error(`Exception Type ${argvtype[i]}, ${argvname[i]} for arg Mapping()`);
        }
    }
    popblock();
    return 1;
}
function transform(argvtype){
    if (typeof argvtype === 'undefined' || argvtype.length == 0) {
        return;
    }
    outputFile(`${' '.repeat(level * 4)}const jerry_value_t rv = jerryx_arg_transform_args(args_p, args_cnt, mapping, JERRYXX_ARRAY_SIZE(mapping));`);
    outputFile(`${' '.repeat(level * 4)}if (jerry_value_is_exception(rv)){`);
    pushblock(`${' '.repeat(level * 4)}}`);
    outputFile(`${' '.repeat((level) * 4)}return rv;`);
    popblock();
    outputFile(`${' '.repeat(level * 4)}JERRYXX_ON_ARGS_COUNT_THROW_ERROR_SYNTAX(args_cnt != ${argvtype.length}, "Wrong arguments count");`);
}
function getNativePtr(classObj){
    outputFile(`\n${' '.repeat(level * 4)}void *native_pointer = NULL;`); lines++; // for newline
    outputFile(`${' '.repeat(level * 4)}native_pointer = jerry_object_get_native_ptr(call_info_p->this_value, &${classObj}_info);`);
}
function callClassObjMethodArgs(methodName,argvname,argvtype){
    let callargs = '';
    callargs+=methodName+'(' ;
    if (typeof argvname=== 'undefined' || argvname.length == 0) {
        callargs+=')';
        return callargs;

    }
    for(let i=0;i<argvname.length;i++){
        if (1){
            callargs+="("+argvtype[i]+') '; // type cast maybe ignore
        }
        callargs+=argvname[i]+((i!=argvname.length-1) ?',':')');
    }
    return callargs;
}
function callClassObjMethod(className,classObj,returnjstype,methodName,argvname,argvtype,argvCtype){
    let rettype  = returnjstype;

    outputFile(`\n${' '.repeat(level * 4)}if(native_pointer) {`); lines++; // for newline
    pushblock(`${' '.repeat(level * 4)}}`);
    outputFile(`${' '.repeat(level * 4)}${className}* ${classObj} = (${className}*)native_pointer;`);
    let callmethod = callClassObjMethodArgs(methodName,argvname,argvtype);

    if (rettype=='void'){
        // call method only then return 
       outputFile(`${' '.repeat(level * 4)}${classObj}->${callmethod};`);
    }else{
       // return by js type
       outputFile(`${' '.repeat(level * 4)}return jerry_${returnjstype}(${classObj}->${callmethod});`);
    }
    popblock();
    outputFile(`${' '.repeat(level * 4)}return jerry_undefined();`);
}

/* return constructor parser obj */
function constructorPrototypeParser(classObjName,prototype) {
    var constructor = {
        className:'',
        classObjName:'',
        methods:[],
        prototype:'TwoWire::TwoWire(uint8_t bus_num)',
        argvCnt:0,
        argvtypes:[],
        argvnames:[],
        jsretobjName:'js_cobj',
    };
    let cobj = constructor;
    cobj.prototype = prototype;
    cobj.classObjName = classObjName;
    // Extract class name
    cobj.className = prototype.split('::')[0].trim();

    // Extract argument types and argument names
    let pattern =/\(\s*\)/; //void 
    if (prototype.match(pattern)){
        cobj.argvCnt = 0;
        return cobj;
    }
    var argsPart = prototype.split('(')[1].split(')')[0];
    var argsList = argsPart.split(',');
    // Extract argument types and names
    console.log(typeof argsPart, typeof argsList);
    if (typeof argsList === 'undefined' || argsPart =='' ){
        cobj.argvCnt = 0;
        return cobj;
    }
    cobj.argvCnt = argsList.length;
    if (argsList.length > 0 && argsList[0].trim() !== '') {
        argsList.forEach(function(arg) {
            var argParts = arg.trim().split(' ');
            cobj.argvtypes.push(argParts[0]);
            cobj.argvnames.push(argParts[1]);
        });
    }
    return cobj;
}
function constructorGenerator(cobj) {
    let prototypeC = cobj.prototype;
    let classObj = cobj.classObjName;
    try {
        genFunctionHeaderPush(classObj,prototypeC,cobj.className);
    //2. mapping argument
            argList(cobj.argvtypes,cobj.argvnames);
            mapping(cobj.argvtypes,cobj.argvnames);
            transform(cobj.argvtypes);
            classObjSetNativePtr(cobj);
            methodsTable(cobj);
            setProperties(cobj);
        genFunctionTailPop();
    } catch (e) {
        // Catch and handle the error
        console.error('Error:', e.message);
    }
    return;
}
function destroyinfoGenerator(cobj) { // put on top
    let classObj = cobj.classObjName;
    outputFile(`${' '.repeat(level * 4)}void ${classObj}_free_callback(void* native_p, jerry_object_native_info_t *info_p);`);

    outputFile(`${' '.repeat(level * 4)}const static jerry_object_native_info_t ${classObj}_info = {`);
    pushblock(`${' '.repeat(level * 4)}};`);

        outputFile(`${' '.repeat(level * 4)}.free_cb = ${classObj}_free_callback`);
    popblock();
}
function destroyFreeCBGenerator(cobj) { // put before constructor
    let className = cobj.className;
    let classObj = cobj.classObjName;
    outputFile(`${' '.repeat(level * 4)}/* ~${className} ${classObj} destroy */`);
    outputFile(`${' '.repeat(level * 4)}void ${classObj}_free_callback(void* native_p, jerry_object_native_info_t *info_p){`);
    pushblock(`${' '.repeat(level * 4)}};`);

    outputFile(`${' '.repeat(level * 4)}WSDEBUG_TPRINTF("[${className}] delete %x\\n",native_p);`);
    outputFile(`${' '.repeat(level * 4)}${className}* ${classObj} = (${className}*)native_p;`);
    outputFile(`${' '.repeat(level * 4)}/** **TODO** free your own resource here **/`);
    // if (!jerry_value_is_undefined(onReceive_cb_func)) 
    //     jerry_value_free(onReceive_cb_func);
    // if (!jerry_value_is_undefined(onRequest_cb_func)) 
    //     jerry_value_free(onRequest_cb_func);    
    outputFile(`${' '.repeat(level * 4)}delete(${classObj});`);
    popblock();
}    


function registerGenerator(cobj) {
    let className = cobj.className;
    let classObj = cobj.classObjName;
    outputFile(`${' '.repeat(level * 4)}bool js_${classObj}_classobj_wrapper(){  `);
    outputFile(`${' '.repeat(level * 4)}//1 a)modified func name and b) define in .h c) call by jswwrap_tc`);
    pushblock(`${' '.repeat(level * 4)}};`);
        outputFile(`${' '.repeat(level * 4)}return jerryx_register_global ("${className}", js_${classObj}_${className});`);
      
        // outputFile(`${' '.repeat(level * 4)}return true;`);
    popblock();
}
function LibdefHeaderGeneratorPush(cobj) {
    let className = cobj.className;
    resetblock();
    outputFile(`#include "jswrap_tc.h"`);
    outputFile(`#ifdef _LANG_JERRYSCRIPT_ `);
    pushblock(`#endif // _LANG_JERRYSCRIPT_ Congraturation Done!`);  level--; // non level shift
    outputFile(`#ifdef _LIB_${className.toUpperCase()}_`);
    pushblock(`#endif //_LIB_${className.toUpperCase()}_`); level--; // non level shift
    outputFile(`/* **HOW 1** simple select copy & paste **/`);
    outputFile(`/* **HOW 2** or using process.argv[2] for output file, then dl file**/`);
    outputFile(`/* **WARNNING** if tab position is wrong, means error found**/`);
    outputFile(`/* **TODO** Include your class declaration here **/`);
    outputFile(`#include "${className}.h"`);

}
function LibdefTailGeneratorPop() {
    popblock(); //#endif //_LIB_${className.toUpperCase()}_
    popblock(); //#endif // _LANG_JERRYSCRIPT_ 
}
function classObjSetNativePtr(cobj){
    let className = cobj.className;
    let classObj = cobj.classObjName;
    let argvnames = cobj.argvnames;
    let argvtypes = cobj.argvtypes;
    let callmehtod = callClassObjMethodArgs(className,argvnames,argvtypes);
    outputFile(`${' '.repeat(level * 4)}${className}* ${classObj} = new ${callmehtod}; //4`);
    outputFile(`${' '.repeat(level * 4)}WSDEBUG_TPRINTF("[${className}] new %x\\n",${classObj});`);
    outputFile(`${' '.repeat(level * 4)}jerry_value_t js_${classObj}obj = jerry_object();`);
    outputFile(`${' '.repeat(level * 4)}jerry_object_set_native_ptr(js_${classObj}obj, &${classObj}_info , ${classObj} );`);
}
function methodsTable(cobj){
    let className = cobj.className;
    let classObj = cobj.classObjName;
    outputFile(`${' '.repeat(level * 4)}// Register initialization function`);
    outputFile(`${' '.repeat(level * 4)}jerryx_property_entry props[] ={ //5 methods`);
        pushblock(`${' '.repeat(level * 4)}};`);
        outputFile(`${' '.repeat(level * 4)}JERRYX_PROPERTY_STRING_SZ ("id", "${className}"),`);
        cobj.methods.forEach((element) => {
            outputFile(`${' '.repeat(level * 4)}JERRYX_PROPERTY_FUNCTION ("${element}", js_${classObj}_${element}),`);
        });
        outputFile(`${' '.repeat(level * 4)}JERRYX_PROPERTY_LIST_END(),`);
        popblock(); 
}
function setProperties(cobj){
    let classObj = cobj.classObjName;
    outputFile(`${' '.repeat(level * 4)}jerryx_register_result reg = jerryx_set_properties (js_${classObj}obj, props);`);
    outputFile(`${' '.repeat(level * 4)}if (jerry_value_is_exception (reg.result)){`);
        pushblock(`${' '.repeat(level * 4)}};`);
    
        outputFile(`${' '.repeat(level * 4)}WSDEBUG_TPRINTF("Only registered %d properties\\r\\n", reg.registered);`);
        outputFile(`${' '.repeat(level * 4)}/* clean up not registered property values */`);
        outputFile(`${' '.repeat(level * 4)}jerryx_release_property_entry (props, reg);`);
        outputFile(`${' '.repeat(level * 4)}jerry_value_free (reg.result);`);
        outputFile(`${' '.repeat(level * 4)}return jerry_undefined();`);
        outputFile(`${' '.repeat(level * 4)}/* clean up the error */`);
        popblock(); 
    outputFile(`${' '.repeat(level * 4)}return js_${classObj}obj;`);
}
/*
 * this generator for method property , check mapping arglist and call obj method
 * TODO callback setting is not generator, need manual dirty work 
 * TODO Byte* array mapping still confuse for jerryx_arg mapping 
 * How to use
 * 1. fill the need field in methods
 *    classObj your objname , for example 'rect'
 *    constructorPrototype :: is needed, for example 'Rectangle::Rectangle(int length,int width);'
 *    prototypesArray
 * 2. upload to tinyconsole
 * 3. js jswmgen [enter]
 * 4. select, copy and paste to file.
 */

let process = require('process');
// TODO for command line mode
// TODO usage js jswmgen methos.json file.cpp 
//  process.argv[2] is json file
//  process.argv[3] is cpp file 
// todo method prototype array using Object.array.forEach to generate all method handles
// Example Wire method prototype generrator 
// Input methods
var methods = {
    classObj : 'rectangle', // TBD write your own object name
    constructorPrototype : 'Rectangle::Rectangle(int length,int width);',  // should be CLASSNAME::CONSTRUCTOR
    prototypesArray : [// TODO will use only C Prototype , suppose JS is auto generated
    // C prototype                                  JS prototype to compare arg type for cast
    'int getSize();',
    'int getLength();',
    'int getWidth();'
    ],
    className :'',
    methodNames:[],

};

const argMappings_C_to_JS = { // TODO for arg_type mappings type 
    "size_t": "int32",
    "int": "int32",
    "int8": "int8",
    "int16": "int16",
    "int32": "int32",
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
    "uint8*": "string",
    "uint8_t*": "string",
    "char*": "string_sz",
    "char": "uint8",
    // "char": "String",
    "double": "number",
    "float": "number",
    "bool": "boolean",
    "void*": "native_pointer", // not support now
    "void": "ignore",        
    "funcptr": "function"   // TODO for callback should be typedef funcptr prototype
};
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
    "char*": "string_sz",
    "char": "number",
    "void": "void"   // undefined is keywordmaybe transfer in functions
};// TODO crettype auto transfer to jsrettype

// todo method prototype array using Object.array.forEach to generate all method handles
// Example Wire method prototype generrator 
// Input methods
var methods = {
    classObj : 'rectangle', // TBD write your own object name
    constructorPrototype : 'Rectangle::Rectangle(int length,int width);',  // should be CLASSNAME::CONSTRUCTOR
    prototypesArray : [// TODO will use only C Prototype , suppose JS is auto generated
    // C prototype                                  JS prototype to compare arg type for cast
    'int getSize();',
    'int getLength();',
    'int getWidth();'
    ],
    className :'',
    methodNames:[],

};

/* main */
let cobj = constructorPrototypeParser(methods.classObj,methods.constructorPrototype);
methods.className = cobj.className;
methods.methodNames=[];
jswGenerator(methods);
  
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
       outputFile(`/*js_${element}*/`);
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
    
    if (returnjstype === ''){
        returnjstype = 'undefined';
        throw new Error(`Exception retType ${returnctype}, js is null`);
    }
    let argvCtype = methodElementsC.argvtypeArray;

    try {
        genFunctionHeaderPush(prototypeC,methodName);

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
function outputFile(statements){
    // if (process.argv[3]!=''){
    //      // append file TODO
    //      return;
    // }
    console.log(statements);
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
    outputFile(s);
}
function genFunctionHeaderPush(prototypeC,methodName){
    outputFile(`//${prototypeC}`); // method prototype
    //1. method prototype
    outputFile(`JS_METHOD(${methodName})`); // method prototype
    outputFile(`{`);
    pushblock(`} /*js_${methodName}*/`);
}
function genFunctionTailPop(){
    popblock();
}
function parserMatch(match) {
    // Extracted elements
    const rettype = match[1];
    const methodName = match[2];
    // Check if the argument list is empty
    if (match[3] === '') {
        // Handle the case of an empty argument list
        const argCnt = 0;
        const argvtypeArray = [];
        const argvnameArray = [];

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
    const pattern = /^(\w+)\s+(\w+)\((.*?)\);$/;
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
        if (argvtype[i]=='uint8_t*'){
            outputFile(`${' '.repeat(level * 4)}${argvtype[i]} ${argvname[i]}[32];`);
        }else{
            outputFile(`${' '.repeat(level * 4)}${argvtype[i]} ${argvname[i]};`);
        }
    } 
    return argvtype.length;   
}
function mappingEntry_string(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_string(${argvname}, sizeof(${argvname}), JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_int(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_${argvtype}(&${argvname}, JERRYX_ARG_CEIL, JERRYX_ARG_NO_CLAMP, JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_function(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}/* **TODO** need to define your c callback to use jerry_call */`);
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_function(&${argvname}, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_coerce_required(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_${argvtype}(&${argvname},  JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_coerce_required(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_${argvtype}(&${argvname},  JERRYX_ARG_NO_COERCE, JERRYX_ARG_REQUIRED),`);
}
function mappingEntry_ignore(argvname,argvtype){
    outputFile(`${' '.repeat(level * 4)}jerryx_arg_ignore (void),`);
}
function mapping(argvtype,argvname){
    if (typeof argvtype=== 'undefined' || argvtype.length == 0) return 0;
    outputFile(`\n${' '.repeat(level * 4)}const jerryx_arg_t mapping[] ={`);
    pushblock(`${' '.repeat(level * 4)}};`);
    let i=0;
    for(i=0;i<argvtype.length;i++) {
        let jsargtype= argMappings_C_to_JS[argvtype[i]];
        switch(jsargtype){// convert c type to mapping
        case 'string':
            mappingEntry_string(argvname[i],argvtype[i]);
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
        case 'function': 
            mappingEntry_function(argvname[i],jsargtype);
            break;
        case 'ignore': 
            mappingEntry_ignore(argvname[i],jsargtype);
            break;
        default:
            throw new Error(`Exception Type ${argvtype[i]}, ${argvname[i]}for arg Mapping()`);
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
    outputFile(`\n${' '.repeat(level * 4)}void *native_pointer = NULL;`);
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

    outputFile(`\n${' '.repeat(level * 4)}if(native_pointer) {`);
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
    var argsPart = prototype.split('(')[1].split(')')[0];
    var argsList = argsPart.split(',');
    // Extract argument types and names
    if (typeof argsList === 'undefined' ){
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
    try {
        genFunctionHeaderPush(prototypeC,cobj.className);
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
    outputFile(`${' '.repeat(level * 4)}bool js_${classObj}_classobj_wraper(){  `);
    outputFile(`${' '.repeat(level * 4)}//1 a)modified func name and b) define in .h c) call by jswwrap_tc`);
    pushblock(`${' '.repeat(level * 4)}};`);
        outputFile(`${' '.repeat(level * 4)}jerryx_register_global ("${className}", js_${className});`);
      
        outputFile(`${' '.repeat(level * 4)}return true;`);
    popblock();
}
function LibdefHeaderGeneratorPush(cobj) {
    let className = cobj.className;
    resetblock();
    outputFile(`#include "jswrap_tc.h"`);
    outputFile(`#ifdef _LANG_JERRYSCRIPT_ `);
    pushblock(`#endif // _LANG_JERRYSCRIPT_ `);  level--; // non level shift
    outputFile(`#ifdef _LIB_${className.toUpperCase()}_`);
    pushblock(`#endif //_LIB_${className.toUpperCase()}_`); level--; // non level shift
    outputFile(`/* **HOW 1** simple select copy & paste **/`);
    outputFile(`/* **HOW 2** or using process.argv[2] for output file, then dl file**/`);
    outputFile(`/* **TODO** Include your class declaration here **/`);
    outputFile(`/* **WARNNING** if tab position is wrong, means error found**/`);
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
    outputFile(`${' '.repeat(level * 4)}// Register initialization function`);
    outputFile(`${' '.repeat(level * 4)}jerryx_property_entry props[] ={ //5 methods`);
        pushblock(`${' '.repeat(level * 4)}};`);
        outputFile(`${' '.repeat(level * 4)}JERRYX_PROPERTY_STRING_SZ ("id", "${className}"),`);
        cobj.methods.forEach((element) => {
            outputFile(`${' '.repeat(level * 4)}JERRYX_PROPERTY_FUNCTION ("${element}", js_${element}),`);
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
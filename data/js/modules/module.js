var modules = {
    id: 'modules',
    cache: {}, // Cache object to store loaded modules
    // nativeModules: {}, // Object to store native modules
    jsScripts: {}, // Object to store JavaScript files
    jsFiles: {}, // Object to store JavaScript files
    /* not work below */
    dumpObject : function (obj) {
        var result='';
        var depth = 0;
        for (let key in obj) {
            if (typeof obj[key] === 'function') {
              // If property is a function, stringify its definition
              result += `${' '.repeat(depth * 2)}${key}: function },\n`;
            } else if (typeof obj[key] === 'object' && obj[key] !== null) {
              // If property is an object, recursively dump it
              result += `${' '.repeat(depth * 2)}${key}: object,\n`;
              this.dumpObject(obj[key]);
            } else {
              // For other types, stringify the value
              result += `${' '.repeat(depth * 2)}${key}: property ${JSON.stringify(obj[key])},\n`;
            }
          }
        },
        listProps :function (obj){
          // Enumerate and list all elements (including functions) in the object
          for (let key in obj) {
            if (obj.hasOwnProperty(key)) {
              console.log(key,"\t",obj[key]); // Print the property name
              // console.log(obj[key]); // Print the property value (function in this case)
            }
          }
        } 
  }; //modules

const listProps = modules.listProps;
const ModuleType = {
  UNKNOWN :   0, /*try type JSFILES 3*/
  CACHE :     1, /* get module and return module.exports */
  // NATIVE :    2, /*cached, C++ wrap global native objects , ie. wsSerial, File, Canvas, ...*/
  JSSCRIPTS:  3, /* define in jsScripts uncache*/
  JSFILES:    4, /* file at path */
  };
function chkModuleType(moduleName){
  if (modules.cache[moduleName])     return ModuleType.CACHE;     /* in cache  */
  // if (modules.nativeModules[moduleName]) return ModuleType.CACHE; /* cache load */
  if (modules.jsScripts[moduleName]) return ModuleType.JSSCRIPTS; /* runtime load */
  if (modules.jsFiles[moduleName]) return ModuleType.JSFILES;     /* runtime load */
  return ModuleType.JSFILES;  // unknow try JSFILES type 
}

function loadScriptsAndEval(moduleName,moduleType){

    var content='';

    // Check if the module is a JavaScript scripts
    if ((moduleType == ModuleType.JSSCRIPTS)||(moduleType == ModuleType.JSFILES ) ) {
        if ((moduleType == ModuleType.JSSCRIPTS ) && modules.jsScripts[moduleName]) {
           content = modules.jsScripts[moduleName];
        }else if ((moduleType == ModuleType.JSFILES ) ) {
           let fpath = "/js/modules/"+moduleName+".js"; //put in /js/Modules
           content = File.readFile(fpath);
        }else{
          throw new Error(`[module.require] '${moduleName}' '${moduleType}' not found`);
          return undefined;
        }
        if (content === '') {
          throw new Error(`[module.require] Module '${moduleName}' not found`);
          return undefined;
        }
        let module = {exports:{ },id:'',content:''};
        modules.cache[moduleName] = module; // cache done
        module.id = moduleName;
        let script = '(function(module) { let exports = module.exports;' + content +
                    ' })(module,module.exports);'  //IIEF return module.exports 
        // module.content = script;
        const result = eval(script);
        if (typeof result === Error) {
          throw new Error(`[module.require] '${result}' `);
          return undefined;
        }
        return module.exports;
    }
    throw new Error(`[module.require] '${moduleName}' not found`);
    return undefined;
  }
function getModule(moduleName) {
    return modules.cache[moduleName];
}
function require(moduleName) {
    // Check if the module is already cached
    let moduletype = chkModuleType(moduleName);
    switch(moduletype) {
      case ModuleType.CACHE :
          return modules.cache[moduleName].exports;
      case ModuleType.JSSCRIPTS:
      case ModuleType.JSFILES:
          // load scripts to eval it and cache , return exports
          return loadScriptsAndEval(moduleName,moduletype);
    }
    // Module not found, throw an error
    throw new Error(`[module.require] '${moduleName}' '${moduletype}' not found`);
}



  // Function to register native modules
  function registerNativeModule(moduleName, nativeObj) {
    if (typeof nativeObj === 'undefined') return;
    const module = {exports:{ },id:''};
    module.exports = nativeObj;
    module.id = moduleName;
    modules.cache[moduleName] = module;
  }
  
  // Function to register JavaScript , no cache
  function registerjsScripts(scriptName, content) {
    modules.jsScripts[scriptName] = content;
  }

  // Function to register JavaScript files, no cache
  function registerjsScriptFile(fileName, fileContent) {
    modules.jsFiles[fileName] = fileContent;
  }
  

const jsScripts = {
  myModule: "const myModule = { hello: function() { return 'Hello from myModule!'; } }; module.exports = myModule;",
  anotherModule: "const anotherModule = { greet: function() { return 'Greetings from anotherModule!'; } };  module.exports = anotherModule;"
};

const jsFiles = {
  // module: 'module', load by native cached
  console: 'console',
  addmodule: 'addmodule'
};

const nativeModules = {
  // module: 'module', load by native cached
  esp: esp32,
  fs: File,
  wifi: wifi,
  sio: wsSerial,
  canvas: Canvas,
  touch: Touch,
  dht: dht,
  mqtt: MqttClient,
  // wire:TwoWire, //I2C
};

  Object.entries(nativeModules).forEach(([moduleName, obj]) => {
    // print('jsFiles',moduleName, obj);
      registerNativeModule(moduleName, obj);
  });

  Object.entries(jsScripts).forEach(([scriptName, scriptContent]) => {
    // print('jsScripts',scriptName, scriptContent);
    registerjsScripts(scriptName, scriptContent);
  });  

  Object.entries(jsFiles).forEach(([fileName, fileContent]) => {
    // print('jsFiles',fileName, fileContent);
    registerjsScriptFile(fileName, fileContent);
  });

  function listdictiony(obj){
    Object.entries(obj).forEach(([key, value]) => {
      print('obj',key, value);
    });    
  } 

  let console = require('console'); //cache xonsole

  // Cache the module object itself during startup move to jswrap_tc code
  // modules.cache['module'] = module;


  // if (typeof module !== 'undefined' && typeof module.exports === 'object' ) {  // run examples
  //    module.exports = {
  //      require
  //    };
  // }else{
  // if (typeof module !== 'undefined' && typeof module.exports === 'object' ) {  // run examples
  // Usage examples with 'require'
if (module === undefined) {
  let myModule = require('myModule');
  console.log(myModule.hello()); // Output: Hello from myModule!
  
  let anotherModule = require('anotherModule');
  console.log(anotherModule.greet()); // Output: Greetings from anotherModule!
  
  // Use cached modules
  let cachedModule = require('module');
  console.log(cachedModule === module); // Output: true (same module instance)
  
  // Try requiring a non-existent module (should throw an error)
  try {
    let nonExistentModule = require('nonExistentModule');
  } catch (error) {
    console.error(error.message); // Output: Module 'nonExistentModule' not found
  }
}
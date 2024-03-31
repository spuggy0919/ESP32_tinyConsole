const modules = {
    cache: {}, // Cache object to store loaded modules
    nativeModules: {}, // Object to store native modules
    jsScripts: {}, // Object to store JavaScript files
    jsFiles: {}, // Object to store JavaScript files

  };
const module = {exports:{}};

function require(moduleName) {
    // Check if the module is already cached
    if (modules.cache[moduleName]) {
      return modules.cache[moduleName];
    }
    // Check if the module is a native module

    // Check if the module is a JavaScript scripts
    if (modules.jsScripts[moduleName]) {
      // Simulate loading module content from a JavaScript file
      print("jsScripts module",moduleName);
      const jsCode = module.jsScripts[moduleName];
      print("jsScripts code",jsCode);
      const moduleContent = eval(jsCode);
      modules.cache[moduleName] = moduleContent;
      return moduleContent;
    }
    let fpath = "/js/modules/"+moduleName+".js";
    let script = File.readFile(fpath);
    print("file",fpath,"\n",script);
                    // id:moduleName,
                    //  path:fpath};
    if (script != "") { // file found
      const obj = eval(script);
      print("obj",obj);
      print("module.exports",module.exports);
      modules.cache[moduleName] = module.exports; // module.exports
      module.exports.log('hello');
      return module.exports;
    }
    // Module not found, throw an error
    throw new Error(`Module '${moduleName}' not found`);
}
        // check nativemodule which bind c++ with registerclassobj  
        // if (module.nativeModules[moduleName]) {
        //   // Load native module
        //   const nativeObj = module.nativeModules[moduleName];
        //   module.cache[moduleName] = nativeObj;
        //   return nativeObj;
        // }
        // Check if the module is a JavaScript scripts
        // if (module.jsScripts[moduleName]) {
        //   // Simulate loading module content from a JavaScript file
        //   print("jsScripts module",moduleName);
        //   const jsCode = module.jsScripts[moduleName];
        //   print("jsScripts code",jsCode);
        //   const moduleContent = eval(jsCode);
        //   module.cache[moduleName] = moduleContent;
        //   return moduleContent;
        // }

        // Module not found, throw an error
        // throw new Error(`Module '${moduleName}' not found`);
  //   }
  // };
        // Check if the module is a JavaScript file
        // if (module.jsFiles[moduleName]) {
        // // Simulate loading module content from a JavaScript file
        //     print("file module",modulename)
        //     const fname = "/js/module/"+module.jsFiles[moduleName];
        //     const jsCode = File.readFile(fname);
        //     const moduleContent = eval(jsCode);
        //     module.cache[moduleName] = moduleContent;
        //     return moduleContent;
        // }
  // Function to register native modules
  // function registerNativeModule(moduleName, nativeObj) {
  //   modules.nativeModules[moduleName] = nativeObj;
  // }
  
  // Function to register JavaScript files
  function registerjsScripts(fileName, fileContent) {
    modules.jsScripts[fileName] = fileContent;
  }

  // Function to register JavaScript files
  function registerjsScriptFile(fileName, fileContent) {
    modules.jsFiles[fileName] = fileContent;
  }
  
  // Example native modules and JavaScript files (in JSON format)
// const nativeModules = {
//   myNativeModule: {
//     method: function() {
//       return 'Native method';
//     }
//   },
//   anotherNativeModule: {
//     greet: function() {
//       return 'Greetings from native object';
//     }
//   },
//   fs : File
// }; // module

//   // Register native modules and JavaScript files
//   Object.entries(nativeModules).forEach(([moduleName, moduleContent]) => {
//     print('nativeModules',moduleName, moduleContent);
//     registerNativeModule(moduleName, moduleContent);
//   });

const jsScripts = {
  myModule: "const myModule = { hello: function() { return 'Hello from myModule!'; } }; module.exports = myModule;",
  anotherModule: "const anotherModule = { greet: function() { return 'Greetings from anotherModule!'; } };  module.exports = anotherModule;"
};

const jsFiles = {
  console: 'console',
  addmodule: 'addmodule'
};


  
  Object.entries(jsScripts).forEach(([scriptName, scriptContent]) => {
    print('jsScripts',scriptName, scriptContent);
    registerjsScripts(scriptName, scriptContent);
  });  

  Object.entries(jsFiles).forEach(([fileName, fileContent]) => {
    print('jsFiles',fileName, fileContent);
    registerjsScriptFile(fileName, fileContent);
  });

  function listdictiony(obj){
    Object.entries(obj).forEach(([key, value]) => {
      print('obj',key, value);
    });    
  } 
  // Cache the module object itself during startup

  // modules.cache['module'] = module;
  modules.cache['fs'] = File;
  modules.cache['sio'] = wsSerial;
  modules.cache['canvas'] = Canvas;
  modules.cache['touch'] = Touch;


  // Usage examples with 'require'
  let console1 = require('console');
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
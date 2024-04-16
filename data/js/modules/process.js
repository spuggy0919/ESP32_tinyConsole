let fs = require('fs');
const configStr = fs.readFile('/config.json');
const config = JSON.parse(configStr);
config.cwd = function () {
    return fs.cd('');
};
config.data = configStr
config.exit = function (code) {
    try {
        if (code != 0) 
           throw new Error(`Exit process (${code})`);
    }catch(e){
        console.error('Error:', e.message);   
        exit(); // call undefined function to exit 
    }
};
if (typeof module === 'undefined') {  // run examples

    console.log(configStr);
    listProps(config);
} else{
    module.exports = config;
    // console.log('module.id',module.id);
}
 

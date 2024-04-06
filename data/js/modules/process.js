let fs = require('fs');
const configStr = fs.readFile('/config.json');
const config = JSON.parse(configStr);
config.cwd = fs.cd('/js');
config.data = configStr
if (typeof module === 'undefined') {  // run examples

    console.log(configStr);
    listProps(config);
} else{
    module.exports = config;
    // console.log('module.id',module.id);
}
 

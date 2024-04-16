let fs = require('fs');
const configStr = fs.readFile('/config.json');
const config = JSON.parse(configStr);
config.cwd = function (){
    return fs.cd();
}
if (typeof module === Object){
    module.export = config
}else{
    console.log(configStr);
    listProps(config);
}
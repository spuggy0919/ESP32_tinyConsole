let fs = require('fs');
// if (fs.id !== 'fs'){
//     console.log('load fs module\n');
//     fs = require('fs');
// }else{
//     fs=fsio;
// }
const Nodefs = Object.assign({},fs,{

    checkNodeJS :function(){
        var platform='TINYCONSOLE'
        if (typeof fs.id === 'undefined') {
            // maybe mac node.js environment
            platform = 'NODEJS'
        }
        return platform;
    },
    
    readFileNodeJS:function(filename){
        if (this.checkNodeJS() == 'NODEJS') {
            try {
                return fs.readFileSync(filename,'utf8');
            }catch(err){
                console.log(err);
            }
        }else{
            return fs.readFile(filename);
        }
    },
    appendFileNodeJS:function(filename, msg){
        if (this.checkNodeJS()  == 'NODEJS') {

            try {
                fs.appendFileSync(filename, msg);
            }catch(err){
                console.log('data append to File',filename,err);
            };
        }else{
            fs.appendFile(filename, msg);
        }
    },
    deleteFileNodeJS:function(filename){
        if (this.checkNodeJS()  == 'NODEJS') {
            if (fs.existsSync(filename)) {
                fs.unlink(filename, function (err) {
                    if (err) throw err;
                    // console.log('File deleted!',filename);
                });
            }
        }else{
            fs.deleteFile(filename);
        }
    },
    writeFileNodeJS:function(filename, msg){
        if (this.checkNodeJS()  == 'NODEJS') {
            try {
            fs.writeFileSync(filename, msg, {flag:'w'});
            }catch(err){
                console.log('data written to File',filename,err);

            };
        }else{
            fs.writeFile(filename, msg);
        }
    }
});
 
if (typeof fsio === 'undefined' && typeof module == 'undefined') {
    console.log(typeof fs);
    console.log(Nodefs.checkNodeJS());
    var outputFile = 'test.txt'
    Nodefs.writeFileNodeJS(outputFile,"Hello World!\n");
    let text1 = 'readback1'+Nodefs.readFileNodeJS(outputFile);
    Nodefs.writeFileNodeJS(outputFile,"Hello World!123\n");
    let text2 = 'readback1'+Nodefs.readFileNodeJS(outputFile);
    console.log('readback',Nodefs.readFileNodeJS(outputFile));
    Nodefs.appendFileNodeJS(outputFile,"Hello World!abc\n");
    let text3 = 'readback1'+Nodefs.readFileNodeJS(outputFile);
    console.log(text1,text2,text3);
    // Nodefs.deleteFileNodeJS(outputFile);
}
module.exports = Nodefs;

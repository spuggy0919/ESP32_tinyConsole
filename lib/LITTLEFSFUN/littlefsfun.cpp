#include "littlefsfun.h"
#include <string.h>


/* 
   version 0.1
    Modified from example of lorol lib =2022.9.28-

   lib_deps =
   lorol/LittleFS_esp32 @ ^1.0.6
   
   
    You only need to format LITTLEFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin 
   
  

   LisDir is display all hierarchy subdirecty 
*/


bool initLittleFS(){ // cd 
#ifdef ESP32
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
#else
    if(!LittleFS.begin()){
#endif
        Serial.println("Error:- LITTLEFS Mount Failed");
        return false;
    }
  return true;
}
// Backwards compatible, <4GB filesystem usage
// struct FSInfo {
//     size_t totalBytes;
//     size_t usedBytes;
//     size_t blockSize;
//     size_t pageSize;
//     size_t maxOpenFiles;
//     size_t maxPathLength;
// };
#ifdef ESP32
#else
FSInfo fs_info;
#endif
String reportfs(fs::FS &fs){
    String ret="";
#ifdef ESP32
    char buf [128];
    sprintf(buf,"totalBytes.  =%8d\nusedBytes    =%8d\nfreeBytes    =%8d\n",\
    LittleFS.totalBytes(),LittleFS.usedBytes(),LittleFS.totalBytes()-LittleFS.usedBytes());
    ret = String(buf);
    Serial.printf("---allocStrleng  =%d\n",ret.length());

    Serial.printf("totalBytes.  =%d\n",LittleFS.totalBytes());
    Serial.printf("usedBytes    =%d\n",LittleFS.usedBytes());
    Serial.printf("freeBytes    =%d\n",LittleFS.totalBytes()-LittleFS.usedBytes());

#else
    if (!fs.info(fs_info)) return String();
    char *buf = (char *) malloc(128);
    sprintf(buf,"totalBytes.  =%8d\nusedBytes    =%8d\nfreeBytes    =%8d\n",\
    fs_info.totalBytes,fs_info.usedBytes,fs_info.totalBytes-fs_info.usedBytes);
    ret = String(buf);
    free(buf);
    Serial.printf("---allocStrleng  =%d\n",ret.length());

    Serial.printf("totalBytes.  =%d\n",fs_info.totalBytes);
    Serial.printf("usedBytes    =%d\n",fs_info.usedBytes);
    Serial.printf("blockSize    =%d\n",fs_info.blockSize);
    Serial.printf("pageSize.    =%d\n",fs_info.pageSize);
    Serial.printf("maxOpenFiles =%d\n",fs_info.maxOpenFiles);
    Serial.printf("maxPathLength=%d\n",fs_info.maxPathLength);
#endif 
    return ret;
    
}
String currentPath = "/"; // path is leading '/' with ending '/'
String checkPath = "/";

String trimPrefix(String path){
    int loc = path.indexOf("/",1);
    if (loc != -1 && path.length()>1){
        return path.substring(loc,path.length());
    }
    return "";
}
// get compoent only without "/"
String getPrefix(String path){
    String ret= "";
    int loc = path.indexOf("/",1);
    if (loc != 0){
        ret =  path.substring(1,loc);
    }
    Serial.println("fsfun:getPrefix:"+ret+" original"+path);
    return ret;
}
String trimLast(String path){ // ..
    String ret = path;

    int loc =path.lastIndexOf("/",path.length()-2);
    if (loc != -1){
        ret = path.substring(0,loc+1);
    }
    Serial.println("fsfun:trimLast:"+ret+" original"+path);
    return ret;
}
bool relativePath(String path) {
    bool ret = !(path.indexOf('/')==0);
     if (ret)   Serial.println("fsfun:relativePath:"+path);
     else Serial.println("fsfun:absolutePath:"+path);
     return ret;
}
bool checkDir(fs::FS &fs, const char *path){
    String tempPath = currentPath;
    String actualPath= "/";
    if (String(path) == "") {
        checkPath = currentPath;
        return true; 
    }
    if  (relativePath(String(path))) { // // relative Path
        tempPath += String(path);
    }else { // absolute path
        tempPath = String(path);
    }
    if (tempPath.lastIndexOf("/",tempPath.length())!=tempPath.length()-1){
        // without last "/" ? 
        // path is leading '/' with ending '/'
        tempPath = tempPath + "/";
    }

    while(tempPath.length()>0){
        Serial.print("fsfun:tempPath"+tempPath);
        Serial.print("fsfun:actualPath"+actualPath);
        String prefix=getPrefix(tempPath);
        tempPath = trimPrefix(tempPath);
        if (prefix == "") continue; 
        if (prefix == ".") continue; // no change
        if (prefix == "..") {
            actualPath = trimLast(actualPath);
            continue;
        }
    #ifdef ESP32
            File file = fs.open((actualPath+prefix).c_str());
    #else
            File file = fs.open((actualPath+prefix).c_str(), FILE_READ);
    #endif         // if (!file->exist()) return String(curpath);
            if (file.isDirectory()){ // change to path 
                actualPath += prefix + "/";
            }else {
                return false; 
            }
    }   
    checkPath  =  actualPath;
    Serial.print("fsfun:tempPath"+tempPath);
    Serial.print("fsfun:actualPath"+actualPath);
    Serial.print("fsfun:checkPath"+checkPath);
   return true; 

}
String changeDir(fs::FS &fs, const char *path){
       if (checkDir(fs, path)){
            currentPath = checkPath;
       }
    Serial.print("fsfun:currentPath"+currentPath);
       return currentPath;
}


String appendFullPath(String path, String name){
    if (name == "") return "";
    if (path== "/") return "/" + name;
    return path + "/" + name;
}
String getlastComponet(const char *pname){
    String last = String(pname);
    int lastidx = last.lastIndexOf('/');
    if (lastidx == last.length()-1) return ""; // ending /
    if (lastidx == -1 ) return last;
    last = last.substring(lastidx+1, last.length());
    if (last == "." || last == "..") return ""; // ending . .. fail
    return last;
}
String getDirname(const char *pname){
    String tpname = String(pname);
    int lastidx = tpname.lastIndexOf('/');
    if (lastidx == -1) return "";
    Serial.printf("dirpath:%d",lastidx);
    return tpname.substring(0, lastidx+1);
}


bool  checkIsDir(fs::FS &fs,const char *pname, bool *dir){
    File f = fs.open(getfullpathFileOrDir(fs,pname));
    if (!f) return false; 
    *dir = f.isDirectory();
    return true;
}
/* check the pname is path or file, if path replace with path apeend fname*/
String  getFullPath_File(const char *pname, const char *fname){
    bool isDir;
    String last =  getlastComponet(fname);
    String prefix = getfullpathFileOrDir(LittleFS, pname);
    if (checkIsDir(LittleFS, prefix.c_str(), &isDir)){
        if (!isDir) return prefix;
        return prefix+last;
        
    }
    return String("");
}

String getfullpathFileOrDir(fs::FS &fs, const char *pname){
    // trim filename
    String ret;
    Serial.printf("getfullpathFileOrDir %s\r\n", pname);
    String last =   getlastComponet(pname);
    if (last == "") return "";
    Serial.println("last:"+last);
    String dirpath = getDirname(pname);
    Serial.println("dirpath:"+dirpath);
    if (checkDir(fs, dirpath.c_str())){
        ret = checkPath + last;
    }else {
        String ret = currentPath+last;
    }
    Serial.println("ret:"+ret);
    return ret;

}

String currentDir(){ // pwd
    return String(currentPath);
}
int32_t levellist = 1;
String printdirentry(File direntry, int32_t levels){
    String retstr = String();
    for(int32_t i = 0; i < levels; i++){
       retstr += "\t";
    }
    if (direntry.isDirectory()){ //  *DIR
        retstr +="*" ;
        retstr += String(direntry.name());
    }else{ // FILE
        retstr += " ";
        retstr += String(direntry.name());
        retstr += "\t";
        retstr += String(direntry.size());
#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
        time_t t= direntry.getLastWrite();
        struct tm * tmstruct = localtime(&t);
        char buffer[40];
        sprintf(buffer,"\t%d-%02d-%02d %02d:%02d:%02d",(tmstruct->tm_year)+1900,( tmstruct->tm_mon)+1, tmstruct->tm_mday,tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
        retstr += String(buffer);
#endif
    }
    retstr +="\r\n";
    return retstr;
}
String dirlistContent = String();
void listDir1(fs::FS &fs,  const char * dirname, uint8_t levels);
String listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    if (!checkDir(fs, dirname)) return "";
    String fdir = checkPath;
    if (fdir == "") return "";
    levellist = levels;
    levels = 0;
    dirlistContent = "directory: "+ fdir +"\r\n";
    listDir1(fs,  fdir.c_str(), levels);
    // Serial.printf("Listing done \r\n");

    return dirlistContent;
}

void listDir1(fs::FS &fs,   const char *  dirname, uint8_t levels){
    // Serial.printf("recursive sub directory: %s \r\n", dirname);    
#ifdef ESP32
    File root = fs.open(dirname);
#else
    File root = fs.open(dirname, FILE_READ);
#endif
    if(!root){
        dirlistContent += "*** failed to open directory\r\n";
        return;
    }
    if(!root.isDirectory()){
        dirlistContent +=  printdirentry(root, levels);
        return;
    }
    File file = root.openNextFile();
    while(file){
        dirlistContent +=  printdirentry(file, levels);
        if (file.isDirectory()){
            if(levels <= levellist){
                char * p = (char*)malloc( strlen( dirname ) + 5 + strlen(file.name()));
                assert( p != NULL );
                strcpy( p, dirname );

                if (strlen(dirname) !=1 ) {
                    p = strcat(p,"/");
                }
                p = strcat(p,file.name());
                listDir1(fs, p, levels + 1 );
               // Serial.printf("recursive return from sub directory: %s \r\n", p);    
                free(p);
            }
        } 
        file.close();
        file = root.openNextFile();
    }
    
    // Serial.printf("recursive return from dirname directory: %s \r\n", dirname);    

}

void createDir(fs::FS &fs, const char * pathi){
    String paths =  getfullpathFileOrDir(fs, pathi);
    const char * path = paths.c_str();
    Serial.printf("Creating Dir: %s\n", path);
    if(!fs.mkdir(path)){
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * pathi){
    String paths =  getfullpathFileOrDir(fs, pathi);
    const char * path = paths.c_str();
    Serial.printf("Removing Dir: %s\n", path);
    if(!fs.rmdir(path)){
        // Serial.println("rmdir failed");
    }
}


String readFile(fs::FS &fs, const char * pathi){
    String paths =  getfullpathFileOrDir(fs, pathi);
    const char * path = paths.c_str();
    //   Serial.printf("Reading file: %s\r\n", path);
#ifdef ESP32
  File file = fs.open(path);
#else
  File file = fs.open(path, FILE_READ);
#endif
  if(!file || file.isDirectory()){
    Serial.printf("ERROR:- empty file or failed to open file\n");
    return String();
  }
//   Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
//   Serial.println(fileContent);
  return fileContent;
}
void writeFile(fs::FS &fs, const char * pathi, const char * message){
    String paths =  getfullpathFileOrDir(fs, pathi);
    const char * path = paths.c_str();
    Serial.printf("Writing file: %s\r\n", path);


    File file = fs.open(path, FILE_WRITE);

    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(!file.print(message)){
        Serial.println("- write failed");
    // } else {
    //     Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * pathi, const char * message){
    String paths =  getfullpathFileOrDir(fs, pathi);
    const char * path = paths.c_str();
        Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        // Serial.println("- failed to open file for appending");
        return;
    }
    if(!file.print(message)){
        Serial.println("- append failed");
    // } else {
    //     Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * pathi1, const char * pathi2){
    String paths =  getfullpathFileOrDir(fs, pathi1);
    const char * path1 = paths.c_str();
    paths =  getfullpathFileOrDir(fs, pathi2);
    const char * path2 = paths.c_str();
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (!fs.rename(path1, path2)) {
        Serial.println("- rename failed");
    // } else {
    //     Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * pathi){
    String paths =  getfullpathFileOrDir(fs, pathi);
    const char * path = paths.c_str();
    Serial.printf("Deleting file: %s\r\n", path);
    if(!fs.remove(path)){
        // Serial.println("- delete failed");
    // } else {
    //     Serial.println("- delete failed");
    }
}

// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

void writeFile2(fs::FS &fs, const char * path, const char * message){
    if(!fs.exists(path)){
		if (strchr(path, '/')) {
            // Serial.printf("Create missing folders of: %s", path);
			char *pathStr = strdup(path);
			if (pathStr) {
				char *ptr = strchr(pathStr, '/'); 
				while (ptr) {
					*ptr = 0;
                    if ( *pathStr!=0 && !fs.exists(pathStr)){
                        // Serial.printf("mkdir: %s ", pathStr); 
					    fs.mkdir(pathStr);
                        if (!fs.exists(pathStr)) {
                            // Serial.printf("fail"); 

                        }
                        // Serial.printf("\r\n"); 

                    }
					*ptr = '/';
					ptr = strchr(ptr+1, '/');
				}
			}
			free(pathStr);
		}
    }

    // Serial.printf("Writing file to: %s with data %s\r\n", path, message);
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        // Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        // Serial.println("- file written");
    } else {
        // Serial.println("- write failed");
    }
    file.close();
}

void deleteFile2(fs::FS &fs, const char * path){
    // Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

    if(fs.remove(path)){
        // Serial.println("- file deleted");
    } else {
        // Serial.println("- delete failed");
    }

    char *pathStr = strdup(path);
    if (pathStr) {
        char *ptr = strrchr(pathStr, '/');
        if (ptr) {
            // Serial.printf("Removing all empty folders on path: %s\r\n", path);
        }
        while (ptr) {
            *ptr = 0;
            fs.rmdir(pathStr);
            ptr = strrchr(pathStr, '/');
        }
        free(pathStr);
    }
}

#ifdef ESP32
#define TESTBLOCK 2048
#else
#define TESTBLOCK 1024
#endif
void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<TESTBLOCK; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", TESTBLOCK * 512, end);
    file.close();
    // listDir(fs,"/",1);
#ifdef ESP32
    file = fs.open(path);
#else
    file = fs.open(path,FILE_READ);
#endif
    start = millis();
    end = start;
    i = 0;
    if(file && (!file.isDirectory())){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.printf("- reading %d" ,len);
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}


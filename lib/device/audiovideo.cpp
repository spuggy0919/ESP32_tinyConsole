
#include "audiovideo.h"


// const functionTable = [
//     createCard,   "0"
//     drawFilledRect,    // drawFilledRect(color, x, y, width, height)
//     drawPixel,         // drawPixel(color, x, y)
//     drawText,          // drawText(color, text, x, y, deg) 
//     drawHLine,         // drawHLine(color, x1, x2, y)
//     drawVLine,         // drawVLine(color, x, y1, y2
//     plotto,            // drawLine(color, x1, y1, x2, y2)
// ];


// const AudiofunctionTable = [
//     playAudio,
//     pauseAudio,
//     stopAudio,
//     seekForward,
//     seekBackward,
//     changeSong, // 5
// ]; 
int video(int argc,char * argv[])
{
    if (argc==1)  {
        videof("0"); // create mode
        return 0;
    }
    String cmd;
    for(int i=1;i<argc;i++){
        cmd += String(argv[i]) + " ";
    }
    Serial.println(cmd);
    videof(cmd.c_str()); // create mode
    return 0;
}

int audio(int argc,char * argv[])
{
    if (argc==1)  {
        audiof("0"); // play mode
        return 0;
    }
    String cmd;
    for(int i=1;i<argc;i++){
        cmd += String(argv[i]) + " ";
    }
    Serial.println(cmd);
    audiof(cmd.c_str()); // create mode
    return 0;
}


int drawText(String text,int x, int y,float deg) { 
    char vbuf[80]; 
    int encodedLength = Base64.encodedLength(text.length());
    char encodedString[encodedLength+1];
    Base64.encode(encodedString, (char *) text.c_str(), text.length());  
    sprintf(vbuf,"%1d %s %d %d %f",DRAW_TEXT,encodedString,x,y,deg); 
    videof(vbuf); 
    return 0;
}
int drawChar(char c) { 
    char vbuf[80]; 
    sprintf(vbuf,"%1d %d",DRAW_CHAR,c); 
    videof(vbuf); 
    return 0;
}

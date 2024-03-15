#ifndef __TinybasicArduino_H__
#define __TinybasicArduino_H__

#define BASIC_V_1_4A
// #define BASIC_V_2_00
#ifdef BASIC_V_1_4A
int tbmain_14a(int argc, char* argv[]);
void tbloop_14a() ;
void tbsetup_14a() ;
#define tbmain tbmain_14a
#define tbloop tbloop_14a
#define tbsetup tbsetup_14a
#else
int tbmain_200(int argc, char* argv[]);
void tbloop_200() ;
void tbsetup_200() ;
#define tbmain tbmain_200
#define tbloop tbloop_200
#define tbsetup tbsetup_200
#endif
#endif 
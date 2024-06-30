#ifndef __MPI_LW_H__
#define __MPI_LW_H__

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncUDP.h>
#else
  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
#endif
#include <ESPmDNS.h>
#include <mutex>
#include <queue>
#include "MPI_Debug.h"



/*---------------------------------------------------------------------------*/
/* WiFi mDNS layer  for ESP32 node discovery                                 */
/*---------------------------------------------------------------------------*/
#define ESPCONSOLE  ("ESPCONSOLE-")       // ESPCONSOLE-XXXX for AP will appen four mac chars
bool WiFimDNS_init();
String WifimDNSName();
int WiFimDNSQuery();
IPAddress WifimDNSLocalIP();

String MDNS_hostname(int i);
IPAddress MDNS_IP(int i);

/*---------------------------------------------------------------------------*/
/* MPI_Iot function                                                          */
/*---------------------------------------------------------------------------*/
int MPI_Iot_LED(int pwmvalue);
int MPI_Iot_LED_Blink(int n);
int MPI_Iot_Restart(int shutdown);
int MPI_Iot_MPICH(const char *cmd);
void MPI_Iot_SetEpoch(unsigned long  Epoch);
unsigned long MPI_Iot_GetEpoch();

/*---------------------------------------------------------------------------*/
/* ESP32 MPI testing                                                         */
/*---------------------------------------------------------------------------*/
// MPI possible public
#define MPI_TUTORIALS 0
#define MPI_MAX_NODES 10
#define MPI_HOSTNAMESIZE 16
#define MPI_IPV4SIZE     16
#define MPINODE_STATUS_NORESPONE (1<<0)
typedef struct {
    char       hostname[MPI_HOSTNAMESIZE]; //ESPCONSOLE-XXXX
    IPAddress  ip;       // 192.168.001.002
    int        status;
}MPI_NODE;
/*---------------------------------------------------------------------------*/
/* udp layer                                                                 */
/*---------------------------------------------------------------------------*/
bool udpserver_init();
size_t udp_Block_Send(uint8_t* data, int length);
/*---------------------------------------------------------------------------*/
/* MPI MSG  API  (format MSG packet, decode, and io function)                */
/*---------------------------------------------------------------------------*/
// RX MSG
bool MPI_MSG_Packet_Dispatch(uint8_t *buffer, int len);
int  MPI_MSG_Config_Parser(uint8_t* data, int len);
// bool MPI_MSG_Ack_Or_Not(AsyncUDPPacket packet);  // RX CHECK to respone ACK
// TX MSG
int  MPI_MSG_Sent_CMD(const char* XXX, ...); //send message packet 
int  MPI_MSG_Scan_And_Config(); // mdns query broad cast 
int  MPI_MSG_Sent_LED(int rank,int onoff); //turn on led on/off
int  MPI_MSG_Sent_RST(int rank,int timeout); // reboot rabk
int  MPI_MSG_Sent_RUN(int rank, int example); // run example on all node
int  MPI_MSG_Sent_PIN(int rank); // ping 
int  MPI_MSG_Sent_PON(int rank); // pong 

typedef int MPI_Status;
#define MPI_SUCCESS 0
#define MPI_FAIL -1

typedef int MPI_Op;

/*---------------------------------------------------------------------------*/
/* MPI_Constant                                                             */
/*---------------------------------------------------------------------------*/
#define MPI_MAX_PROCESSOR_NAME 32
#define BROADCAST_RANK 255

/*---------------------------------------------------------------------------*/
/* MPI_Datatype                                                              */
/*---------------------------------------------------------------------------*/

typedef int MPI_Datatype;
#define MPI_DATATYPE_NULL           ((MPI_Datatype)0x0c000000)

#define MPI_CHAR                    ((MPI_Datatype)0x4c000101)
#define MPI_UNSIGNED_CHAR           ((MPI_Datatype)0x4c000102)
#define MPI_SHORT                   ((MPI_Datatype)0x4c000203)
#define MPI_UNSIGNED_SHORT          ((MPI_Datatype)0x4c000204)
#define MPI_INT                     ((MPI_Datatype)0x4c000405)
#define MPI_UNSIGNED                ((MPI_Datatype)0x4c000406)
#define MPI_LONG                    ((MPI_Datatype)0x4c000407)
#define MPI_UNSIGNED_LONG           ((MPI_Datatype)0x4c000408)
#define MPI_LONG_LONG_INT           ((MPI_Datatype)0x4c000809)
#define MPI_LONG_LONG               MPI_LONG_LONG_INT
#define MPI_FLOAT                   ((MPI_Datatype)0x4c00040a)
#define MPI_DOUBLE                  ((MPI_Datatype)0x4c00080b)
#define MPI_LONG_DOUBLE             ((MPI_Datatype)0x4c00080c)
#define MPI_BYTE                    ((MPI_Datatype)0x4c00010d)
#define MPI_WCHAR                   ((MPI_Datatype)0x4c00020e)

// javascript
#define MPI_NUMBER                  ((MPI_Datatype)0x4c001001)
#define MPI_INTEGER                 ((MPI_Datatype)0x4c001002)
#define MPI_STRING                  ((MPI_Datatype)0x4c001004)
#define MPI_BOOLEAN                 ((MPI_Datatype)0x4c001008)
#define MPI_ARRAYBUF                ((MPI_Datatype)0x4c001008)
#define MPI_BIGINT                  ((MPI_Datatype)0x4c001010)

/*---------------------------------------------------------------------------*/
/* MPI_Comm                                                                  */
/*---------------------------------------------------------------------------*/

typedef int MPI_Comm;
#define MPI_COMM_NULL  ((MPI_Comm)0x04000000)

#define MPI_COMM_WORLD ((MPI_Comm)0x44000000)
#define MPI_COMM_SELF  ((MPI_Comm)0x44000001)

/*---------------------------------------------------------------------------*/
/* _MPI_API  private   also IO control                                                      */
/*---------------------------------------------------------------------------*/
int  _MPI_Config_Dump();
void MPI_printf(const char *fmt,...);
int _MPI_Run_Example(int example);
extern int _mpi_state;
/*---------------------------------------------------------------------------*/
/* MPI_API  subset                                                           */
/*---------------------------------------------------------------------------*/
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
int MPI_Barrier(MPI_Comm comm);
int MPI_Comm_rank(MPI_Comm comm, int *rank);
int MPI_Comm_size(MPI_Comm comm, int *size);
int MPI_Finalize(void);
int MPI_Get_processor_name(char *name, int *resultlen);
int MPI_Init(int *argc, char ***argv);
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);



#endif // __MPI_H__

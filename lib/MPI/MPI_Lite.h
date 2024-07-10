#ifndef __MPI_LITE_H__
#define __MPI_LITE_H__

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

extern const uint16_t glistenPort;
extern AsyncUDP udpServer;
extern AsyncUDP udpClient;
extern std::mutex udpmutex,upd_seq_mutex,upd_dispatch_mutex,mallocMutex;

extern uint32_t mpi_comm_size;
extern uint32_t mpi_comm_rank;


extern int _mpi_state; // config or not with mpinode
/*---------------------------------------------------------------------------*/
/* MPI_MSG Packet define                                                     */
/*---------------------------------------------------------------------------*/
// udp packet header
#define PACKET_TYPE_SIZE 4
// CFG
// RUN
// ACK rank   rank respone ack
// LED rank 0|1   turn on off led[rank]
// RST rank       reboot
// PRT MESSAGE    printf
// struct PacketHeader {
//     char     pmsg[PACKET_TYPE_SIZE];
//     uint8_t  seqno;
//     uint8_t  from;
//     uint8_t to;  // signed byte reserver
//     uint8_t trycnt;
// #ifdef _PACKET_EXTENSION_
//     uint32_t reserve; //  for
// #endif
// };


typedef int MPI_Status;
#define MPI_SUCCESS 0
#define MPI_FAIL -1


#define MSG_MAX_RETRIES 3  // Timeout in milliseconds
#define MSG_TIMEOUT_MS 3000  // Timeout in milliseconds

#define MSG_MAX_NODES  MPI_MAX_NODES    // Maximum number of nodes (0-254, 255 reserved for broadcast)

#define MSG_MAX_LENGTH 256
#define MSG_MAX_STRING 128
#define MSG_INT_SIZE   sizeof(int)
#define MSG_ACTION_NOACK (1<<0)

#define MSG_NONE_REPLY      (1<<0)
#define MSG_NEED_REPLY_ACK  (1<<1)
#define MSG_NEED_REPLY_PONG (1<<2)
#define MSG_NEED_REPLY_DATA (1<<3)
#define MSG_PACKET_ACK_PONG (MSG_NEED_REPLY_ACK|MSG_NEED_REPLY_PONG)
#define MSG_PACKET_REPLY    (MSG_NEED_REPLY_ACK|MSG_NEED_REPLY_PONG|MSG_NEED_REPLY_DATA)
#define MSG_DATA_PACKET     (1<<4) 

#define MSG_STATE_NULL      (0) 
#define MSG_STATE_OUTGOING  (1<<1) 
#define MSG_STATE_ACKING_PONGING   (1<<2) 
#define MSG_STATE_RETRY     (1<<2) 
#define MSG_STATE_ACK_PON   (1<<3) 
#define MSG_STATE_WAITING   (1<<4) 
#define MSG_STATE_MATCH     (1<<5) 
#define MSG_STATE_TIMEOUT   (1<<7) 


#define MSG_PAYLOAD_SIZE_DEFAULT 0
typedef struct {
    // message code 
    char     pmsg[PACKET_TYPE_SIZE];
    // route
    uint8_t  from;  // from rank
    uint8_t  to;    // to rank
    uint8_t  comm;    // communicator
    uint8_t  tag;    // tag
    uint32_t seqno;  // for ack check
    // ack mechanism
    uint32_t ackmask; // answer
    uint32_t timeStamp; // for timeout
    uint32_t remoteip; // send local ip
    uint8_t  answer; // answer
    // data
    // uint8_t  encrypt;
    uint8_t  checksum;
    uint32_t  count;
    uint32_t  typecode;
    size_t  length; // Actual length of the payload
    uint8_t  payload[MSG_PAYLOAD_SIZE_DEFAULT];
} MPI_Packet;
#define MPI_PACKET_PAYLOAD_SIZE(x) (x-sizeof(MPI_Packet)) // x is length
// ack tracking
typedef struct {
    MPI_Packet *pkt;
    SemaphoreHandle_t ackSemaphore;
    uint16_t   ackstate;
    uint16_t   pktstate;
    long       lasttime;
    uint8_t    trycnt; // Retry count for retry    // uint32_t localip; //  receive remote ip
}MPI_AckContext;
typedef struct {
    MPI_Packet *pkt;
    SemaphoreHandle_t matchSemaphore;
    int status;
}MPI_RecvContext;

//Message Code Format Size Table
typedef struct{
    int  size;
    const char *msg;
    const char *fmt;
    uint8_t answer_needed; // may define in MESSAGE coding table
    // const char *replymsg; // may define in MESSAGE coding table
}MSGCODE_ITEM;
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
bool MPI_Iot_Setup();
int MPI_Iot_LED(int pwmvalue);
int MPI_Iot_LED_Blink(int onoff, int ledpwm);
int MPI_Iot_Restart(int shutdown);
int MPI_Iot_MPIRUN(const char *cmd);
void MPI_Iot_SetEpoch(unsigned long  Epoch);
unsigned long MPI_Iot_GetEpoch();
/*---------------------------------------------------------------------------*/
/* ESP32 MPI testing                                                         */
/*---------------------------------------------------------------------------*/
// MPI possible public
#define MPI_TUTORIALS 0
#define MPI_MAX_NODES 16            // Maximum number of nodes (0-254, 255 reserved for broadcast)
#define MPI_HOSTNAMESIZE 16
#define MPI_IPV4SIZE     16
#define MPINODE_STATUS_NORESPONE (1<<0)
typedef struct {
    char       hostname[MPI_HOSTNAMESIZE]; //ESPCONSOLE-XXXX
    IPAddress  ip;       // 192.168.001.002
    int        status;
}MPI_NODE;



typedef int MPI_Op;
typedef int MPI_Request;
/*---------------------------------------------------------------------------*/
/* udp layer                                                                 */
/*---------------------------------------------------------------------------*/
bool MPI_udp_init();
int udp_Block_Send(MPI_Packet *packet);
int udp_Block_Recv(MPI_Packet *packet);
uint32_t _IPAddressToUInt(IPAddress ip);
void udp_FreeAllRequests(); // finalize
bool udp_isMsgPacket(MPI_Packet *packet, const char *msg);
MPI_Request *udp_Async_Send_Packet(MPI_Packet *packet);
MPI_Request *udp_Async_Recv_Packet(MPI_Packet *packet);
int udp_Test(MPI_Request *request, int *flag, MPI_Status *status);

/*---------------------------------------------------------------------------*/
/* MPI MSG  API  (format MSG packet, decode, and io function)                */
/*---------------------------------------------------------------------------*/




// RX MSG
bool MPI_MSG_Packet_Dispatch(MPI_Packet *packet);
int  MPI_MSG_Config_Parser(uint8_t* data, int len);
// bool MPI_MSG_Ack_Or_Not(AsyncUDPPacket packet);  // RX CHECK to respone ACK
// TX MSG
MPI_Packet* MPI_MSG_Create_Packet(const char* XXX, ...); //create message packet 
void MPI_MSG_Free_Packet(MPI_Packet *packet); //send message packet 

int  MPI_MSG_Scan_And_Config(); // mdns query broad cast 
int  MPI_MSG_Sent_COM(int rank,int size); //broadcast comm_size
int  MPI_MSG_Sent_LED(int rank,int onoff); //turn on led on/off
int  MPI_MSG_Sent_RST(int rank,int timeout); // reboot rabk
int  MPI_MSG_Sent_RUN(int rank,char *example); // run example on all node
int  MPI_MSG_Sent_PIN(int rank); // ping 
int  MPI_MSG_Sent_PON(int rank); // pong 
int  MPI_MSG_Sent_DBG(int rank,int debug); //turn on debug mode 





/*---------------------------------------------------------------------------*/
/* MPI_Constant                                                             */
/*---------------------------------------------------------------------------*/
#define MPI_MAX_PROCESSOR_NAME 32
#define MPI_BROADCAST_RANK 255

/*---------------------------------------------------------------------------*/
/* MPI_Datatype                                                              */
/*---------------------------------------------------------------------------*/

typedef int MPI_Datatype;
#define MPI_DATATYPE_NULL           ((MPI_Datatype)0x0c000000)

#define MPI_SIZEOF(x)               (((x)&(MPI_Datatype)0x00ff00)>>8)
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
// int _MPI_Run_Example(int example);
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
int MPI_Abort(MPI_Comm comm, int errorcode);
int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status);

#endif // __MPI_LITE_H__

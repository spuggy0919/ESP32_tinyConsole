// TODO hash header.ptye to indirect call packet udp dispatch function
#include "MPI_Lite.h"            // TODO port as arduino library 
// #include "wifiinit.h"       // TODO wifi mdns function should be move here 

// ***********UDP**************
AsyncUDP udpServer;
AsyncUDP udpClient;
const uint16_t glistenPort = 8987;


uint32_t _udp_send_seq=0;
uint32_t _udp_arrive_seq=0;
#define PACKETHEADER 8 // pcket headersize if MPI_Packet change, modify it



// MPINODE STATUS
#define MPI_NODE_STATE_INIT   0x0000 // root scan all nodes
#define MPI_NODE_STATE_SIZE   0x0001 // node size broadcast
#define MPI_NODE_STATE_CONFIG 0x0002 // config done, rank is assigned
#define MPI_NODE_STATE_ROOT   0x0004 // config done,as root
#define MPI_NODE_STATE_ACK    0x8000 // ack bit
#define MPI_NODE_STATE_PON    0x4000 // ack bit
static int mpi_initialized = 0;
int _mpi_state = 0;
MPI_NODE *mpinode=NULL;
std::mutex udpmutex,upd_seq_mutex,upd_dispatch_mutex;
uint32_t mpi_comm_size = 0;
#define RANK_INIT 0 // initial config for node
uint32_t mpi_comm_rank = RANK_INIT;

uint16_t MPI_debugLevel = 0;//MPI_DBG_UDP |MPI_DBG_API; //MPI_DBG_UDPDUMP| MPI_DBG_ERROR  | MPI_DBG_WARNING | MPI_DBG_PINGPONG | MPI_DBG_MSG;//| MPI_DBG_UDP ;//| MPI_DBG_INFO  | MPI_DBG_MSG ;//| MPI_DBG_UDP| MPI_DBG_MSG| MPI_DBG_API; // Enable error and warning messages
std::mutex debugMutex;


bool upd_chkdebug(int x)
{
    std::lock_guard<std::mutex> lock(debugMutex);
    bool ret = MPI_debugLevel&x;
    return ret;
}
// udp server receive packet handle
void udp_dump_packet(const char* msg,uint8_t *data, int len){
    MPI_DBG_CHK_RET_VOID(MPI_DBG_UDPDUMP);

    MPI_PRINTF("%s ",msg);
    
    for(int i=0;i<len;i++){
        
        if ((i)%8==0) {
            MPI_PRINTF("\n%04x  ",i);
        }
        MPI_PRINTF("%02x ",data[i]);

    }
    MPI_PRINTF("\n");
}
uint32_t _IPAddressToUInt(IPAddress ip)
{
    uint32_t i = ((uint32_t)ip[3]<<24) | ((uint32_t)ip[2]<<16) | ((uint32_t)ip[1]<<8) | ((uint32_t)ip[0]);
    return i;
}

/*---------------------------------------------------------------------------*/
/* MPI_MSG layer, which format and decode Packet                             */
/*---------------------------------------------------------------------------*/

// _ private IPAddress convert
String _formatIPAddress(IPAddress ipAddress) {
  String formattedIP;
  char buffer[20];
  sprintf(buffer,"%03d.%03d.%03d.%03d",ipAddress[0],ipAddress[1],ipAddress[2],ipAddress[3]);
//   MPI_DBG_MSG_PRINTF("_formatIPAddress(%s)%03d.%03d.%03d.%03d\n",buffer,ipAddress[0],ipAddress[1],ipAddress[2],ipAddress[3]);
  return String((char *)buffer);
//   for (int i = 0; i < 4; i++) {
//     if (i > 0) {
//       formattedIP += ".";
//     }
//     formattedIP += String(ipAddress[i], DEC);
//     if (ipAddress[i] < 10) {
//       formattedIP += "00";
//     } else if (ipAddress[i] < 100) {
//       formattedIP += "0";
//     }
//   }
//   return formattedIP;
}
IPAddress _parseIPAddress(String formattedIP) {
  byte octets[4];
  
  // Split the formatted IP address string into octets
  int index = 0;
  for (int i = 0; i < 4; i++) {
    int value = 0;
    while (formattedIP[index] != '.' && index < formattedIP.length()) {
      value = value * 10 + (formattedIP[index] - '0');
      index++;
    }
    octets[i] = value;
    index++;
  }
  IPAddress ipAddress(octets[0],octets[1],octets[2],octets[3]);

  return ipAddress;
}


typedef struct {
    uint32_t        typever;
    MPI_Datatype    typecode;
    short           typesize;
    short           typecount;
    short           typereserved;
    uint8_t         typespace[0];
}TYPEARRAY;

/**
   @brief RX MSG packet parser handler
   @details
         When receive a MSG Packet, it is check the XXX code, now the PIN and ACK is filtered
         Only data or IO command packet is arrived

   @param[in] XXX MSGCODE, 
                   >0  is single and specific node rank. (must be less than mpi_comm_size)
   @param[in] ...  rank and arglist
   @return true: for ack, else no reply, but now is check in onpacket 
 */
// TODO hash header.ptye to indirect call dispatch function
// TODO retrieve RX_pkt_Queue, to examine, then  excute IO function or data processing
static uint8_t *_recvbuf; // RXT
bool MPI_MSG_Packet_Dispatch(MPI_Packet *packet){
    // Determine packet type
    // MPI_DBG_MSG_PRINTF("[RX]dispatch(%s)[%d](%d,%d)\n",packet->pmsg,packet->seqno,packet->from,packet->to);

    if (strncmp(packet->pmsg, "CFG", PACKET_TYPE_SIZE) == 0) {
        // Parse the configuration
        // mpi_comm_rank is assign by COM packet each
        if (mpi_comm_rank == 0 && (_mpi_state & MPI_NODE_STATE_CONFIG)) return true; // root is always config
        MPI_MSG_Config_Parser((uint8_t *)packet, packet->length);
    } else if (strncmp(packet->pmsg, "COM", PACKET_TYPE_SIZE) == 0) {
        int size;
        memcpy(&size, packet->payload, sizeof(int)); 
        MPI_DBG_MSG_PRINTF("(rank:%d) size(%x)\n",packet->to,size); 
        mpi_comm_size = size;
        mpi_comm_rank = packet->to;
        _mpi_state |= MPI_NODE_STATE_SIZE;
    } else if (strncmp(packet->pmsg, "RUN", PACKET_TYPE_SIZE) == 0) {
        // Handle the "RUN" packet, run example at each node
           packet->payload[packet->length-1]=0;
        String cmd = String((const char *)(packet->payload));
        cmd += "\n";
        // memcpy(&example, buffer+offset, sizeof(int)); offset+=sizeof(int);
        // if (example>MPI_TUTORIALS) example = 0;
        MPI_DBG_MSG_PRINTF("(rank:%d) example(%s)\n",mpi_comm_rank,cmd.c_str()); 
        MPI_Iot_MPIRUN(cmd.c_str());


        // _MPI_Run_Example(example);
        // Implement logic for "Run" packet
#if 0 // move to rxTask
    } else if ((strncmp(packet->pmsg, "ACK", PACKET_TYPE_SIZE) == 0) || \
               (strncmp(packet->pmsg, "PON", PACKET_TYPE_SIZE) == 0)) {
        // Handle the "ACK" packet
        int remoteRank = packet->from;
        // memcpy(&remoteRank, buffer+offset, sizeof(uint32_t)); offset+=sizeof(uint32_t);
        if (mpinode != NULL && remoteRank < mpi_comm_size) {
            udpmutex.lock();
            mpinode[remoteRank].status|=MPI_NODE_STATE_ACK;
            udpmutex.unlock();
        }
        // IPAddress ip(packet->remoteip);
        // Serial.printf("%s(from ip %s rank:from(%d,%d)ip:%s\n",packet->pmsg,ip.toString().c_str(),packet->from,packet->to);
        return false;
        // Implement logic for "Run" packet
#endif
    } else if (strncmp(packet->pmsg, "PRT", PACKET_TYPE_SIZE) == 0) {
           packet->payload[packet->length-1]=0;
           MPI_PRINTF((const char *)(packet->payload)); // redirect IO to root
    } else if (strncmp(packet->pmsg, "TXT", PACKET_TYPE_SIZE) == 0) {
           int bufsize =  packet->length - sizeof(MPI_Packet); 
           memcpy(_recvbuf,packet->payload, bufsize);                         
    } else if (strncmp(packet->pmsg, "RXT", PACKET_TYPE_SIZE) == 0) {
           int ptr;
           memcpy(&ptr, packet->payload, sizeof(int)); 
           _recvbuf = (uint8_t *)ptr;
           MPI_DBG_MSG_PRINTF("RXT(ptr:%x) %x %c\n",ptr, _recvbuf, _recvbuf[0]); 
    } else if (strncmp(packet->pmsg, "LED", PACKET_TYPE_SIZE) == 0) {
        int onoff;
        memcpy(&onoff, packet->payload, sizeof(int)); 
        MPI_DBG_MSG_PRINTF("(rank:%d) onoff(%x)\n",mpi_comm_rank,onoff); 
        MPI_Iot_LED(onoff);
    } else if (strncmp(packet->pmsg, "DBG", PACKET_TYPE_SIZE) == 0) {
        uint16_t debug;
        memcpy(&debug, packet->payload, sizeof(uint16_t));
        MPI_DBG_MSG_PRINTF("(rank:%d) debug(%x)\n",mpi_comm_rank,debug); 
        debugMutex.lock();
        MPI_debugLevel = debug;   
        debugMutex.unlock();

    } else if (strncmp(packet->pmsg, "STM", PACKET_TYPE_SIZE) == 0) {
        unsigned long epoch;
        memcpy(&epoch, packet->payload, sizeof(unsigned long));
        MPI_DBG_MSG_PRINTF("(rank:%d) epoch(%ul)\n",mpi_comm_rank,epoch); 
        MPI_Iot_SetEpoch(epoch);    
    } else if (strncmp(packet->pmsg, "RST", PACKET_TYPE_SIZE) == 0) {
        // Handle the "RST" packet
        uint32_t timeout;
        memcpy(&timeout, packet->payload, sizeof(int));  
        MPI_Iot_Restart(timeout);
    } 
    return true; // for free
}

/**/


/*---------------------------------------------------------------------------*/
/* mdns_ LAN Discovery to build Config Table                                 */
/* udp_ send receive message layer                                           */
/*---------------------------------------------------------------------------*/
bool _MPI_MSG_Config_Broadcast( MPI_NODE nodes[], int nodeCount) {
    int offset = 0;
    MPI_DBG_MSG_PRINTF("_MPI_MSG_Config_Broadcast(size:%d)----Create----\n",nodeCount);
    mallocMutex.lock();
    unsigned char *buffer= (unsigned char *)malloc((sizeof(MPI_NODE)+MPI_IPV4SIZE)*nodeCount+sizeof(MPI_Packet)+100);
    mallocMutex.unlock();

    MPI_Packet header = {"CFG"};
    header.from = 0;
    header.to = MPI_BROADCAST_RANK; // broadcast
    
    memcpy(buffer + offset, &header, sizeof(MPI_Packet)); 
    offset+=sizeof(MPI_Packet);
    memcpy(buffer + offset, &nodeCount, sizeof(int));  
    offset+=sizeof(int);
    for (int i = 0; i < nodeCount; i++) {
        nodes[i].status = 0;
        memcpy(buffer + offset, &i, sizeof(int)); // index as rank
        offset += sizeof(int);
        memcpy(buffer + offset, nodes[i].hostname, MPI_HOSTNAMESIZE);
        offset +=  MPI_HOSTNAMESIZE;
        String fmtip =  _formatIPAddress(nodes[i].ip);
        memcpy(buffer + offset, fmtip.c_str(), MPI_IPV4SIZE);
        offset += MPI_IPV4SIZE;
    }
    MPI_Packet *packet = (MPI_Packet *)buffer;
    packet->answer = MSG_NEED_REPLY_ACK;
    packet->length=offset;
    int ret = udp_Block_Send(packet);
    MPI_MSG_Free_Packet(packet);
    if (ret == MSG_STATE_ACK_PON)  return true; // config

    return false;
}

int MPI_MSG_Scan_And_Config(){  // broadcase 
  int n=WiFimDNSQuery();
  int esp32nodes = 0;
    esp32nodes++;

  if (n == 0) {
        MPI_DBG_WARNING_PRINTF("[mDNS] scan No others services found\n");
  } else {
    for (int i = 0; i < n; ++i) {
      if (String(MDNS_hostname(i).substring(0,11)) == String(ESPCONSOLE)) {
        esp32nodes++;
      }
    }
  }
  // wsTextPrintf("%d mdnsQuery found and %d services found\n",n,esp32nodes);
  // alloc mpinode to store 
      mallocMutex.lock();
  if (mpinode!=NULL) free(mpinode);
  mpinode = (MPI_NODE*) malloc(sizeof(MPI_NODE)*esp32nodes);
      mallocMutex.unlock();

  if (mpinode == NULL) return -1;
  int idx=0;
    strncpy(mpinode[idx].hostname, WifimDNSName().c_str(),MPI_HOSTNAMESIZE-1);  mpinode[idx].hostname[MPI_HOSTNAMESIZE-1]=0;
    // strncpy(mpinode[idx].ip, _formatIPAddress(WifimDNSLocalIP()).c_str(),MPI_IPV4SIZE-1);  mpinode[idx].ip[MPI_IPV4SIZE-1]=0;
    mpinode[idx].ip = WifimDNSLocalIP(); 
    mpinode[idx].status = MPI_NODE_STATE_INIT;
    // wsTextPrintf("%2d:%04x:",idx,mpinode[idx].status); 
    // wsTextPrintf("%s:",mpinode[idx].hostname);
    // wsTextPrintf("%s\n",mpinode[idx].ip.toString().c_str());
    idx++;
  if (esp32nodes == 1) {
  } else {
    for (int i = 0; i < n; ++i) {
      if (String(MDNS_hostname(i).substring(0,11)) == String(ESPCONSOLE)) {
        strncpy(mpinode[idx].hostname, MDNS_hostname(i).c_str(),MPI_HOSTNAMESIZE-1);  mpinode[idx].hostname[MPI_HOSTNAMESIZE-1]=0;
        mpinode[idx].ip = MDNS_IP(i); 
        mpinode[idx].status = MPI_NODE_STATE_INIT;
        // wsTextPrintf("%2d:%04x:",idx,mpinode[idx].status); 
        // wsTextPrintf("%s:",mpinode[idx].hostname);
        // wsTextPrintf("%s\n",mpinode[idx].ip.toString().c_str());  
        idx++;        
        // You can now use the IP address to communicate with the matched service
      }
    }
  }
  mpi_comm_size = idx;
  mpi_comm_rank = 0;
  _mpi_state = MPI_NODE_STATE_CONFIG|MPI_NODE_STATE_ROOT;
  _MPI_Config_Dump();
  MPI_Iot_LED_Blink(1,255);
  // according ackmask is bitslice, then each node should know its rank, so we can not broadcast
  // only inform by ip not rank with root configuration for ack control flow 
  for(int i=1; i<mpi_comm_size;i++) {
      MPI_MSG_Sent_COM(i, mpi_comm_size); // inform client size rank first 
  }
  // config
  _MPI_MSG_Config_Broadcast(mpinode, mpi_comm_size);
  MPI_Iot_LED_Blink(0,255);

  return MPI_SUCCESS;
}
int MPI_MSG_Config_Parser(uint8_t* data, int dataSize) {
    int offset = sizeof(MPI_Packet);
    int esp32nodes;
    memcpy(&esp32nodes, data + offset, sizeof(int));
    mpi_comm_size = esp32nodes; // MPI_size
    offset+=sizeof(int);   
    if (esp32nodes<0||esp32nodes>MPI_MAX_NODES) return -1;
    // alloc mpinode to store 
    mallocMutex.lock();
    if (mpinode!=NULL) free(mpinode); // free old 
    mpinode = (MPI_NODE *) malloc(sizeof(MPI_NODE)*esp32nodes);
    mallocMutex.unlock();

    if (mpinode == NULL) return -1;
    String consoleStr = "ESPCONSOLE-XXXX";
    String fmtip = "192.168.001.001";
    MPI_Iot_LED_Blink(1,255);
    for(int i=0;i<esp32nodes;i++){
        int rank;
        char ip[MPI_IPV4SIZE]; ip[MPI_IPV4SIZE-1]=0;
        memcpy(&rank, data + offset, sizeof(int));
        offset += sizeof(int);
        mpinode[i].status = 0;
        memcpy(mpinode[i].hostname, data + offset, MPI_HOSTNAMESIZE); mpinode[i].hostname[MPI_HOSTNAMESIZE-1]=0;
        offset +=  MPI_HOSTNAMESIZE;
        memcpy(ip, data + offset, MPI_IPV4SIZE);
        offset += MPI_IPV4SIZE;
        mpinode[i].ip = _parseIPAddress(String(ip));
        if (String(mpinode[i].hostname) == WifimDNSName()) {
            mpi_comm_rank = i; // MPI_rank assign rank
        }
        if (mpinode[i].ip == WifimDNSLocalIP()) {
            mpi_comm_rank = i; // MPI_rank assign rank
        }
    }
    // MPI_DBG_MSG_PRINTF("[udp_Config_Parser](size:%d,rank:%d)\n",mpi_comm_size,mpi_comm_rank);
    _mpi_state = MPI_NODE_STATE_CONFIG; // not root
    _MPI_Config_Dump();
    MPI_Iot_LED_Blink(0,255);

    return mpi_comm_size;
}
// TODO convert code
#define MSG_CODE_CFG 0
#define MSG_CODE_DBG 1
#define MSG_CODE_DAT 2
#define MSG_CODE_LED 3
#define MSG_CODE_PRT 4
#define MSG_CODE_PIN 5
#define MSG_CODE_PON 6
#define MSG_CODE_RST 7  
#define MSG_CODE_RUN 8
#define MSG_CODE_ACK 9
#define MSG_CODE_NAK 10
MSGCODE_ITEM msgformat[]= {
    // fmt: type 1 CCCC RANK(i)     Size MPI_Packet
    // fmt: type 2 CCCC RANK(i) int Size MPI_Packet+4
    // fmt: type 2 CCCC RANK(i) int Size MPI_Packet+4
                    0, "CFG", "i" ,    MSG_NEED_REPLY_ACK, //0 CFG rank(int) 
    MSG_INT_SIZE, "COM", "ii" ,    MSG_NEED_REPLY_ACK, //0 COM rank(int) size(int)
    MSG_INT_SIZE, "DBG", "ii",    MSG_NEED_REPLY_ACK,//1 DBG rank(int) debugflag(int)
    MSG_MAX_STRING, "DAT", "in",    MSG_NEED_REPLY_ACK,//1 DBG rank(int) debugflag(int)
    MSG_INT_SIZE, "LED", "ii",    MSG_NEED_REPLY_ACK,//2 LED rank(int) onoff(int)
    MSG_MAX_STRING, "PRT", "is" ,   MSG_NEED_REPLY_ACK,// RODO i + PRT rank(int) string(char *)
                     0, "PIN", "i" ,    MSG_NEED_REPLY_PONG,//3 PING rank(int) Epoch(unsigned long)
                     0, "PON", "i" ,    MSG_NONE_REPLY, //4 PONG rank(int) Epoch(unsigned long)
    MSG_INT_SIZE, "RST", "ii" ,   MSG_NEED_REPLY_ACK,  //5 RES rank(int) onoff(int)
    MSG_MAX_STRING, "RUN", "is",    MSG_NEED_REPLY_ACK,  //6 RUN rank(int) tutorial(int)
                    // set time maybe merge into ACK seqcode replace with timecode 
                    // MSG_INT_SIZE, "TST", "ii",    MSG_NEED_REPLY_ACK,  //7 RUN rank(int) tutorial(int)
                    // MSG_INT_SIZE, "STM", "it",    MSG_NEED_REPLY_ACK,//7 STM rank(int) Epoch(unsigned long) SET Epoch >>> <<<AXK
                    // MSG_INT_SIZE, "GTM", "ii",    MSG_NEED_REPLY_DTM,//7 STM rank(int) Epoch(unsigned long) GET Epoch >>> <<<ACK Epoch
                    // MSG_INT_SIZE, "DTM", "ii",     MSG_NEED_REPLY_ACK,//7 STM rank(int) Epoch(unsigned long) ACK Epoch 
                     0, "ACK", "i",     MSG_NONE_REPLY,   //7 RUN rank(int) tutorial(int)
                     0, "NAK", "i",     MSG_NONE_REPLY,   //7 RUN rank(int) tutorial(int)
    MSG_MAX_STRING+MSG_INT_SIZE,   "OPN", "irs",    MSG_NEED_REPLY_ACK, //7 OPEN request or sent BEGIN with filename
                     0, "EOF", "i",     MSG_NEED_REPLY_ACK,    //7  ENO OF FILE
                     4, "SEG", "ii",    MSG_NEED_REPLY_ACK, //7 segment typecode request or sent BEGIN with filename
                     0, "EOS", "i",     MSG_NEED_REPLY_ACK,    //7  ENO OF SEG
    MSG_MAX_LENGTH, "CUK", "i",     MSG_NEED_REPLY_ACK,    //7  CHUNK
    MSG_MAX_LENGTH-4*sizeof(int), "TXD", "in",     MSG_NEED_REPLY_ACK,    //7  TXD rank {comm tag  n typecode databufer ...}
                   6*sizeof(int), "RXD", "in",     MSG_NONE_REPLY, //7  SEND rank {{ tag comm n typecode buferptr}
    MSG_MAX_LENGTH+5*sizeof(int), "TXT", "in",     MSG_NEED_REPLY_ACK,    //7  dump TXT rank {comm tag  n typecode databufer ...}
    MSG_MAX_LENGTH+5*sizeof(int), "RXT", "in",     MSG_NONE_REPLY,    //7  dump RXT rank {comm tag  n typecode databufer ...}
                    -1, ""   , "" ,     MSG_NONE_REPLY
};
/*
    typical only one MSCODE is need for sent, if MSG is too long, like File transfer or long data array
    the OPEN BEGIN is start, like a open file, or segmentation start, CHUNK data sent
    then follow a DAT or FIL type
    the may raw data with se
    then EOF, 
 */
typedef union{
    uint8_t  uint8_v;
    uint16_t uint16_v;
    uint32_t  uint32_v;
    unsigned long     ulong_v;
    long     long_v;
    float    float_v;
    double   double_v;
    char*    string_v;
    int*     int_ptr;
}TYPESPACE;

int MPI_MSG_TypeCode_sizeof(MPI_Datatype code){
    return MPI_SIZEOF(code);
    switch(code){
        case  MPI_CHAR          :   return(sizeof(char));
        case  MPI_UNSIGNED_CHAR :   return(sizeof(unsigned char));
        case  MPI_SHORT         :   return(sizeof(short));
        case  MPI_UNSIGNED_SHORT:   return(sizeof(unsigned short));
        case  MPI_INT           :   return(sizeof(int));
        case  MPI_UNSIGNED      :   return(sizeof(unsigned));
        case  MPI_LONG          :   return(sizeof(long));
        case  MPI_UNSIGNED_LONG :   return(sizeof(unsigned long));
        case  MPI_LONG_LONG_INT :   return(sizeof(long long int));
        // case  MPI_LONG_LONG     :   return(sizeof(long long));
        case  MPI_FLOAT         :   return(sizeof(float));
        case  MPI_DOUBLE        :   return(sizeof(double));
        case  MPI_LONG_DOUBLE   :   return(sizeof(long double));
        case  MPI_BYTE          :   return(sizeof(uint8_t));
        case  MPI_WCHAR         :   return(sizeof(uint16_t));

#if 0
        // javascript
        case  MPI_NUMBER        :   return(sizeof());
        case  MPI_INTEGER       :   return(sizeof());
        case  MPI_STRING        :   return(sizeof());
        case  MPI_BOOLEAN       :   return(sizeof());
        case  MPI_ARRAYBUF      :   return(sizeof());
        case  MPI_BIGINT        :   return(sizeof());
#endif       
        default:
        break;
    }
    return sizeof(uint8_t); //TODO mapping MPI_Datatype to sizeof
}
#define MPI_TYPE_VER 0x0001
/**
   @brief create a MSG packet
   @details
         MPI_Packet defined the whole packet content, the length is depend on data length
         if DATA type and content with string, it is maximum 128 bytes for MPI_IOT limitation
         1. match the item of MSGCODE table with XXX 3 byte chars, 
         2. get fmt string to using va_arg convert arg types, and write into MPI_Packet
        // TODO 3. now is Block_sent, may split Created_Packet and Send seperate  
        // TODO File or segment maybe another function to process chunk
   @param[in] XXX MSGCODE, 
                   >0  is single and specific node rank. (must be less than mpi_comm_size)
   @param[in] ...  rank and arglist
   @return bytes: for sucessful, sent bytes, else 0
 */
std::mutex createtMutex;
MPI_Packet* MPI_MSG_Create_Packet(const char* XXX, ...) { //send message packet 
    std::lock_guard<std::mutex> lock(createtMutex); // avoid reentry

    int offset = 0; // created packet size
    // exception , CFG
    String message = String(XXX);
    if (message==String("CFG")) {
        MPI_MSG_Scan_And_Config();
        return NULL;
    }
    // search fmt table
    int i; // matched index scope out of for 
    for(i=1;i<sizeof(msgformat)/sizeof(MSGCODE_ITEM);i++){
        if (msgformat[i].size<0) return NULL; // not found
        if (message == String(msgformat[i].msg)) break;
    }

    // arglist 
    va_list args;
    va_start(args, XXX);
    // found match item, read to parser FMT string to get args
    // 1.pmsg XXXX
    MSGCODE_ITEM *pmsg = &msgformat[i];
    const char* fmt=pmsg->fmt; 
    // 2.rank
    int rank =  (int)  va_arg(args, int); fmt++; //skip rank // first is rank, default 
    MPI_DBG_MSG_PRINTF("MPI_MSG_Create_Packet()----------Packet Create-----\n");
    MPI_DBG_MSG_PRINTF("  MPI_MSG_Create_Packet()%s rank:%d \n",XXX,rank);


    // packet  alloced,  0, if only rank in header, rank is filled into from
    mallocMutex.lock();
    uint8_t *buffer=(uint8_t *)malloc(pmsg->size+sizeof(MPI_Packet)); 
    mallocMutex.unlock();

    // packet 
    MPI_Packet *packet = (MPI_Packet *)buffer; 
    memcpy(packet->pmsg,message.c_str(),4);
    packet->from = mpi_comm_rank;
    packet->to = rank;
    packet->answer = pmsg->answer_needed;
    packet->length = pmsg->size+sizeof(MPI_Packet);
    if ((packet->to != MPI_BROADCAST_RANK) && ((_mpi_state&MPI_NODE_STATE_CONFIG)!=0) ) { // if config, remoteIP setup from mpi_node
        packet->remoteip =  _IPAddressToUInt(mpinode[packet->to].ip);
        // packet->remoteip=_IPAddressToUInt(WifimDNSLocalIP());
    } else if ((packet->to == 0)){
        packet->remoteip=_IPAddressToUInt(WifimDNSLocalIP());
    }   
    offset = offsetof(MPI_Packet,payload);  
    int idx = 0, length = -1; 
    TYPEARRAY mpi_type = {0x0001,-1,-1,-1};
    int strlen = 0;

    TYPESPACE type_argv;
    // prepare arglist 
    int fmtchar = *fmt++; 
    uint8_t *buf;
    int str_len=0;
    while(fmtchar != 0) {
        // mpi_type.typecount = 0; // array item count prefetch, otherwise 1
        mpi_type.typesize = -1; //reset n;
        switch(fmtchar){
        case 'n': // next arg will be array array lengths next char as array type 
            // n is a data structure which for irecv and isent
            // 1.tag
            packet->tag = (uint8_t)  va_arg(args, int); //skip rank // first is rank, default 
            MPI_DBG_MSG_PRINTF("[MSG]txt(tag:%d)\n",packet->tag);
            // 2.comm
            packet->comm =  (uint8_t)   va_arg(args, int);  //skip rank // first is rank, default 
            MPI_DBG_MSG_PRINTF("[MSG]txt(comm:%d)\n",packet->comm);
            // 3.count  at payload offset
            packet->count =  va_arg(args, int);  
            MPI_DBG_MSG_PRINTF("[MSG]txt(count:%d)\n",packet->count);
            // 4.type size
            packet->typecode  =  va_arg(args, int);  
            MPI_DBG_MSG_PRINTF("[MSG]txt(typecode:%x)\n",packet->typecode);
            mpi_type.typesize = MPI_SIZEOF(packet->typecode)*packet->count;
            MPI_DBG_MSG_PRINTF("[MSG]txt(typesize:%d)\n",mpi_type.typesize);
            // 5.buffer
            buf = (uint8_t *)va_arg(args, char *);
            MPI_DBG_MSG_PRINTF("[MSG]txt(buf:%c)at address%x\n",buf[0],&buf[0]);
            if (udp_isMsgPacket(packet,"TXD") || udp_isMsgPacket(packet,"TXT") ) {
                MPI_DBG_MSG_PRINTF("[MSG]txt(msg:%s)\n",packet->pmsg);
                if ( mpi_type.typesize  > pmsg->size){
                    MPI_DBG_ERROR_PRINTF("MPI_MSG_Create_Packet()%s rank:%d exceed maximun size, data loss\n",XXX,rank);
                     mpi_type.typesize = pmsg->size;
                }
                memcpy(buffer + offset, buf, mpi_type.typesize);  offset+=mpi_type.typesize;
                if(udp_isMsgPacket(packet,"TXT")){
                    MPI_DBG_MSG_PRINTF("[MSG]txt(tag:%d,comm:%d,count:%d,typecode:%x,size%d,%s)\n",\
                        packet->tag,packet->comm,packet->count,packet->typecode,mpi_type.typesize,packet->payload);
                }
            }else if (udp_isMsgPacket(packet,"RXD") || udp_isMsgPacket(packet,"RXT")) {
                //  copy buffer pointer into packet for later grabe data
                int  ptr = (int) &buf[0];
                MPI_DBG_MSG_PRINTF("[MSG]txt(buf:%c)at address%x\n",((uint8_t*)ptr)[0],ptr);
                memcpy(buffer + offset, (uint8_t *)&ptr, sizeof(uint8_t *));  offset+=sizeof(uint8_t *);
            }
            mpi_type.typesize = -1; // reset type size to avoid memcpy
            break;
        case 'b': // uint8_t byte
            mpi_type.typesize = sizeof(uint8_t);
            type_argv.uint8_v = (uint8_t) va_arg(args, int);
            break;
        case 'w': // short wchar 
            mpi_type.typesize = sizeof(uint16_t);        
            type_argv.uint16_v = (uint16_t)va_arg(args, int);
            break;
        case 'i': // int array lengths next char as array type 
            mpi_type.typesize = sizeof(uint32_t);        
            type_argv.uint32_v = va_arg(args, uint32_t);
            MPI_DBG_MSG_PRINTF("[MSG]format(i)%d\n",type_argv.uint32_v);
            break;
        // case 'f': // float
        //     mpi_type.typesize = sizeof(float);        
        //     type_argv.float_v = va_arg(args, float);
        //     break;
        case 'd': // double
            mpi_type.typesize = sizeof(double);        
            type_argv.double_v = va_arg(args, double);
            break;
        case 'l': // double
            mpi_type.typesize = sizeof(long);        
            type_argv.long_v = va_arg(args, long);
            break;
        case 't': // double
            mpi_type.typesize = sizeof(unsigned long);        
            type_argv.ulong_v = MPI_Iot_GetEpoch();
            break;
        // case 'L': // long long
        //     mpi_type.typesize = sieof(long long);        
        //     break;
        case 's': // c string null endding
             type_argv.string_v = (char *)va_arg(args, char *); //type_argv.string_v[5]=0;
             mpi_type.typecode = MPI_CHAR;
             mpi_type.typecount = String(type_argv.string_v).length()+1; // null endding
             if (mpi_type.typecount > MSG_MAX_STRING)  mpi_type.typecount = MSG_MAX_STRING-2;
             mpi_type.typesize = sizeof(uint8_t)*mpi_type.typecount;
             MPI_DBG_MSG_PRINTF("MPI_MSG_Create_Packet()len:%d string %s\n",mpi_type.typecount,type_argv.string_v);// TODO type_argv.string_v);
            //  memcpy(buffer + offset, &mpi_type, sizeof(TYPEARRAY));  offset+=sizeof(TYPEARRAY);
             memcpy((char *)(buffer + offset), type_argv.string_v, mpi_type.typesize);
             offset += sizeof(uint16_t)+mpi_type.typesize;  
             mpi_type.typesize = -1;
            break;   
        default:
            mpi_type.typesize = -1;
            MPI_DBG_ERROR_PRINTF("[MSG]Unsupport format char(%c)\n",fmtchar);
            break; 
        // case 'S': // Arduino String
        //     strlen = strcpy(buffer + offset,String(args[i++])); 
        //     offset+= strlen;
        //     mpi_type.typesize = -1
        //     break;        
        }
        if (mpi_type.typesize != -1) { // one item
            memcpy(buffer + offset, &type_argv, mpi_type.typesize); 
            offset+= mpi_type.typesize;       
        }
        fmtchar=*fmt++;
    }
    va_end(args);
    packet->length =  offset;
    udp_dump_packet("\n[MPI][MSG]MPI_MSG_Create_Packet()\n", (uint8_t *)packet, packet->length);
    return packet;
}

void MPI_MSG_Free_Packet(MPI_Packet *packet) { //send message packet 
    // udp_dump_packet("\n[MPI][MSG]MPI_MSG_Free_Packet()\n", (uint8_t *)packet, packet->length);
    MPI_DBG_MSG_PRINTF("****(%s)***********Packet life endding*****************\n",packet->pmsg); 
    free(packet);
}
/**
   @brief TX MSG packet testing API 
   @details
         These API Sent dedicate MSGCODE to excute Iot Function
            int MPI_Iot_LED(int pwmvalue);
            int MPI_Iot_LED_Blink(int blink, int led);
            int MPI_Iot_Restart(int shutdown);
            int MPI_Iot_MPIRUN(const char *cmd);
            // void MPI_Iot_SetEpoch(unsigned long  Epoch);
            // unsigned long MPI_Iot_GetEpoch();

   @param[in] rank packet is sent to wworld rank, 255 is broadcasr, 
   @param[in] data  parameters for MSGCODE, 

   @return bytes: for sucessful, sent bytes, else 0
 */
// if < 0 fail timeout 
int MPI_MSG_Sent_DBG(int rank,int debug) { //turn on debug mode 
    MPI_DBG_MSG_PRINTF("[LED](rank:%d,debug:%d)\n",rank,debug); 
    MPI_Packet *packet = MPI_MSG_Create_Packet("DBG",rank,debug);
    // send mpipacket 
    int n = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
int MPI_MSG_Sent_COM(int rank,int size) { //turn on led on/off
    MPI_DBG_MSG_PRINTF("[COM](rank:%d,onoff:%d)\n",rank,size); 
    MPI_Packet *packet = MPI_MSG_Create_Packet("COM",rank,size);
    // send mpipacket 
    int status = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
// if < 0 fail timeout 
int MPI_MSG_Sent_LED(int rank,int onoff) { //turn on led on/off
    MPI_DBG_MSG_PRINTF("[LED](rank:%d,onoff:%d)\n",rank,onoff); 
    MPI_Packet *packet = MPI_MSG_Create_Packet("LED",rank,onoff);
    // send mpipacket 
    int status = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
int MPI_MSG_Sent_RST(int rank,int timeout) { // reboot rabk
    MPI_DBG_MSG_PRINTF("[RST](rank:%d) timeout(%d)\n",rank,timeout); 
    MPI_Packet *packet = MPI_MSG_Create_Packet("RST",rank,timeout);
    // send mpipacket 
    int status = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
int MPI_MSG_Sent_RUN(int rank, char *example) { // run on all node
    MPI_DBG_MSG_PRINTF("[RUN](rank:%d,example:%d)\n",rank,example); 
    MPI_Packet *packet = MPI_MSG_Create_Packet("RUN",rank,example);
    // send mpipacket 
    int status = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
int MPI_MSG_Sent_PIN(int rank) { // ping 
    MPI_DBG_PINGPONG_PRINTF("PING%d\n",rank);
    MPI_Packet *packet = MPI_MSG_Create_Packet("PIN",rank);
    // send mpipacket 
    int status = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
int MPI_MSG_Sent_PON(int rank) { // pong 
    MPI_DBG_PINGPONG_PRINTF("PONG%d\n",rank);
    MPI_Packet *packet = MPI_MSG_Create_Packet("PON",rank);
    // send mpipacket 
    int status = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
/*---------------------------------------------------------------------------*/
/* _MPI_API private                                                          */
/*---------------------------------------------------------------------------*/

int _MPI_Config_Dump() {
    if (_mpi_state == 0) {
            MPI_DBG_WARNING_PRINTF("_MPI_Config_dump(size:%d,rank:%d)Unconfig\n",mpi_comm_size,mpi_comm_rank); 
            return -1;
    }
    MPI_DBG_API_PRINTF("[_MPI_Config_dump]state=%x(size:%d,rank:%d)\n",_mpi_state,mpi_comm_size,mpi_comm_rank); 
    for (int i = 0; i < mpi_comm_size; ++i) {
        MPI_PRINTF((i==mpi_comm_rank) ? "*":" ");
        MPI_PRINTF("%2d:%04x:",i,mpinode[i].status); 
        MPI_PRINTF("%s:",mpinode[i].hostname);
        MPI_PRINTF("%s\n",mpinode[i].ip.toString().c_str());  
      }        
      return MPI_SUCCESS;
}
// // static link and execute example
// int cmd_mpi_hello(int argc,char *argv[]);
// int _MPI_Run_Example(int example) {
//     int argc = 1;
//     char *argv[]={(char *)"mhello"};
//     if (mpinode==NULL ) { //unconfig 
//         return -1;
//     }
//     switch(example){
//     case 0:
//     default:
//         cmd_mpi_hello(argc,argv);
//         break;
//     }
//     return MPI_SUCCESS;
// }

#include <string.h>
#if 0
// API and also MSG layers
void MPI_printf(const char *fmt,...){
  int sizebuf = strlen(fmt)+100;
  char *buffer=(char *)malloc(sizeof(MPI_Packet)+MSG_MAX_STRING);
  int offset = 0;
    MPI_Packet *packet = (MPI_Packet *)buffer;
    strncpy(buffer,"PRT",PACKET_TYPE_SIZE);
    packet->from = mpi_comm_rank;
    packet->to = 0; // TODO ?? 
    offset = offsetof(MPI_Packet,payload);  
  va_list args;
  va_start(args, fmt);
  int length = vsnprintf(buffer+offset, sizebuf, fmt, args); 
  char *pstr = buffer+offset;
  offset+=length; buffer[offset++]=0;
  int n=0;
  va_end(args);  
    
        // udp_dump_packet("\n[MPI][MSG]MPI_printf()\n", (uint8_t*)buffer, offset);
        packet->length=offset;
        n = udp_Block_Send(packet); // MPI_printf 


  MPI_DBG_MSG_PRINTF("[MSG]MPI_printf()(%d,%d)len%d %d %s\n",packet->from,packet->to,offset,n,pstr);
  free(buffer);

}
#else
// API and also MSG layers
// print message to rank 0
void MPI_printf(const char *fmt,...){
    int sizebuf =256;
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    int length = vsnprintf(buffer, sizebuf, fmt, args); 
    va_end(args);  
    MPI_Packet *packet =  MPI_MSG_Create_Packet("PRT",0,buffer);
    int status = udp_Block_Send(packet); // MPI_printf 
    MPI_MSG_Free_Packet(packet);
    return;
}


#endif
/*---------------------------------------------------------------------------*/
/* MPI_API subset                                                            */
/*---------------------------------------------------------------------------*/
int MPI_Init(int *argc, char ***argv) {
    mpi_initialized = 0;
    // Normally, you would parse command-line arguments and set up the environment here.
    int ret = MPI_SUCCESS;
    if (_mpi_state == 0) { // force config
        ret = MPI_MSG_Scan_And_Config();
    } 
    if (ret != MPI_SUCCESS) return ret;
    mpi_initialized = 1;
    MPI_Iot_LED_Blink(mpi_initialized,255);

    MPI_DBG_API_PRINTF("[MPI_Init](%d)\n",*argc);
    return MPI_SUCCESS;
}
int MPI_Finalize(void) {
    mpi_initialized = 0;
    MPI_DBG_API_PRINTF("[MPI_Finalize]()\n");
    udp_FreeAllRequests();
    // if (mpinode) free(mpinode);
    // mpi_comm_size = 0;
    // mpi_comm_rank = MPI_BROADCAST_RANK;

    // wait for _udp_arrive_seq == _udp_send_seq

#define UDPCOMPLETETIMEOUT 5000
    // unsigned long lastTime = millis();
    // unsigned long  currentTime = millis();
    MPI_MSG_Sent_PIN(255);
    MPI_MSG_Sent_PIN(255); // toogle led, too
    // while ((currentTime - lastTime) < UDPCOMPLETETIMEOUT) {
    //     bool complete_pon = false;
    //     upd_seq_mutex.lock();
    //     complete_pon = (_udp_arrive_seq ==  _udp_send_seq);
    //         MPI_DBG_API_PRINTF("[MPI_Finalize](%d,%d)\n",_udp_send_seq,_udp_arrive_seq);

    //     upd_seq_mutex.unlock();
    //     if (complete_pon) break;
    //     currentTime = millis();
    // }
    MPI_Iot_LED_Blink(mpi_initialized,255);

    return MPI_SUCCESS;
}

int MPI_Comm_size(MPI_Comm comm, int *size) {
    if (!mpi_initialized) return -1;
    *size = mpi_comm_size;
    MPI_DBG_API_PRINTF("[MPI_Comm_size](%d)\n",*size);
    return MPI_SUCCESS;
}
int MPI_Comm_rank(MPI_Comm comm, int *rank) {
    if (!mpi_initialized) return -1;
    *rank = mpi_comm_rank;
    MPI_DBG_API_PRINTF("[MPI_Comm_rank](%d)\n",*rank);
    return MPI_SUCCESS;
}
int MPI_Get_processor_name(char *name, int *resultlen){
    if (!mpi_initialized) return -1;
    int len = sprintf(name,"NODE%02d",mpi_comm_rank);
    // memcpy(name,mpinode[mpi_comm_rank].hostname,MPI_HOSTNAMESIZE);
    *resultlen = len;
    MPI_DBG_API_PRINTF("[MPI_Send]MPI_Get_processor_name(%s,%d)\n",name,*resultlen);
    return MPI_SUCCESS;
}


int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
    if (!mpi_initialized) return -1;
    // Simplified send: In a real implementation, you'd use network communication
    MPI_DBG_API_PRINTF("[MPI_Send] %d items to process %d\n", count, dest);
    MPI_Packet *packet = MPI_MSG_Create_Packet("RXD",dest,tag,comm,count,datatype,buf);
    // send mpipacket 
    int status = udp_Block_Send(packet);   
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status) {
    if (!mpi_initialized) return -1;
    // Simplified receive: In a real implementation, you'd use network communication
    MPI_DBG_API_PRINTF("[MPI_Recv]Receiving %d items from process %d\n", count, source);
    MPI_DBG_PINGPONG_PRINTF("Recv%d\n", source);
    MPI_Packet *packet = MPI_MSG_Create_Packet("RXD",source,tag,comm,count,datatype,buf);
    // send mpipacket 
    // TODO how to return the matching result back to mpi 
    *status = udp_Block_Recv(packet); // buffer is set if successful
    MPI_MSG_Free_Packet(packet);
    return MPI_SUCCESS;
}
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request){
    if (!mpi_initialized) return MPI_FAIL;
    MPI_DBG_API_PRINTF("[MPI_Irecv]Receiving %x %d items from process %d\n", &buf, count, source);
        //                                      RXD  rank   tag comm count typcode  buffer
    MPI_Packet *packet = MPI_MSG_Create_Packet("RXD",source,tag,comm,count,datatype,buf);
    // packet is free by udp_test
    *request = udp_Async_Recv_Packet(packet);
    return MPI_SUCCESS;

}

int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request){
    if (!mpi_initialized) return MPI_FAIL;
        //                                      TXD  rank tag comm count typcode  buffer
    MPI_Packet *packet = MPI_MSG_Create_Packet("TXD",dest,tag,comm,count,datatype,buf);
    *request = udp_Async_Send_Packet(packet);

    return MPI_SUCCESS;

}

int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status){
    if (!mpi_initialized) return MPI_FAIL;
    int ret = udp_Test(request, flag, status);
    if (*flag) { // free send packet

    }
    return ret;
}



int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    if (!mpi_initialized) return -1;
    // Simplified broadcast: In a real implementation, you'd use network communication
    MPI_DBG_API_PRINTF("[MPI_Bcast]Broadcasting %d items from process %d\n", count, root);
    return MPI_SUCCESS;
}

int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
    if (!mpi_initialized) return -1;
    // Simplified reduce: In a real implementation, you'd use network communication and reduction operation
    MPI_DBG_API_PRINTF("[MPI_Reduce]Reducing %d items to process %d\n", count, root);
    return MPI_SUCCESS;
}
int MPI_Barrier(MPI_Comm comm) {
    if (!mpi_initialized) return -1;
    // Simplified barrier: In a real implementation, you'd synchronize processes
    MPI_DBG_API_PRINTF("[MPI_Reduce]Barrier synchronization\n");
    return MPI_SUCCESS;
}


void interpreter_exit(int errorcode); // TODO external reference interpreter exit function

int MPI_Abort(MPI_Comm comm, int errorcode) {
    if (!mpi_initialized) return -1;
    mpi_initialized = 0;
    // Simplified barrier: In a real implementation, you'd synchronize processes
    udp_FreeAllRequests();
    MPI_DBG_API_PRINTF("[MPI_Abort]abort (%d))\n",errorcode);
    interpreter_exit(errorcode); // TODO external reference interpreter exit function
    MPI_Iot_LED_Blink(mpi_initialized,255);
    return MPI_SUCCESS;
}



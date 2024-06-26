// TODO hash header.ptye to indirect call packet udp dispatch function
#include "MPI.h"            // TODO port as arduino library 
#include "wifiinit.h"       // TODO wifi mdns function should be move here 
#include "ledpwm.h"         // TODO pwm led for ESP32 using arduino IO




// ***********UDP**************
AsyncUDP udpServer;
AsyncUDP udpClient;
const uint16_t listenPort = 8987;

// ***********PACKET MESSAGE**************
// udp packet header
#define PACKET_TYPE_SIZE 4
// CFG
// EXE
// ACK rank   rank respone ack
// LED rank 0|1   turn on off led[rank]
// RST rank       reboot
// PRT MESSAGE    printf
struct PacketHeader {
    char ptype[PACKET_TYPE_SIZE];
    uint8_t seqno;
    uint8_t from;
    uint8_t to;
    uint8_t trycnt;
};
#define PACKETHEADER 8 // pcket headersize if PacketHeader change, modify it




// MPINODE STATUS
#define MPI_NODE_STATE_INIT   0x0000 // root scan all nodes
#define MPI_NODE_STATE_CONFIG 0x0001 // config done, rank is assigned
#define MPI_NODE_STATE_ROOT   0x0002 // config done,as root
#define MPI_NODE_STATE_ACK    0x8000 // ack bit
static int mpi_initialized = 0;
int _mpi_state = 0;

uint16_t MPI_debugLevel =  MPI_DBG_ERROR | MPI_DBG_ERROR | MPI_DBG_WARNING | MPI_DBG_INFO  | MPI_DBG_UDP ;//| MPI_DBG_MSG| MPI_DBG_API; // Enable error and warning messages
std::mutex MPI_debugmutex;


MPI_NODE *mpinode=NULL;
std::mutex udpmutex;
static uint32_t mpi_comm_size = 2;
#define RANK_INIT 0 // initial config for node
static uint32_t mpi_comm_rank = RANK_INIT;



// udp server receive packet handle
void udp_dump_packet(const char* msg,uint8_t *data, int len){
    MPI_DBG_CHK_RET_VOID(MPI_DBG_UDP);

    MPI_PRINTF("%s ",msg);
    
    for(int i=0;i<len;i++){
        
        if ((i)%8==0) {
            MPI_PRINTF("\n%04x  ",i);
        }
        MPI_PRINTF("%02x ",data[i]);

    }
    MPI_PRINTF("\n");
}

int udp_Send_Ack(AsyncUDPPacket packet); //sent ack to remote ip

void udpserver_onPacket(AsyncUDPPacket packet) {
    MPI_DBG_UDP_PRINTF("[RX]onPacket:frm:%s,(%d,%d)\n",packet.remoteIP().toString().c_str(),sizeof(PacketHeader),sizeof(int));
    // Determine packet type
    if (MPI_MSG_Packet_Dispatch(packet.data(), packet.length())){
            udp_Send_Ack(packet);
    }
    // MPI_MSG_Ack_Or_Not(packet);
    udp_dump_packet("[UDP][RX]",packet.data(), packet.length());
    
}
// udp server handle 
bool udpserver_init() {
   
 // Setup UDP Server
    if (udpServer.listen(listenPort)) {
        MPI_DBG_UDP_PRINTF("Listening on IP: %s, Port: %d\n", WifiLocalIP().c_str(), listenPort);
        // Register the callback function to handle incoming packets
        udpServer.onPacket(udpserver_onPacket);
    }
    return true;
}

// client handle
// bool udpclient_Connect(IPAddress serverIP, uint16_t port) {

//     MPI_DBG_INFO_PRINTF("[UDP] send to server %s:port%d\n",serverIP.toString().c_str(),port);
//     if (udpClient.connect(serverIP, port)) {
//         wsTextPrintf("[UDP][TX]:");
//         udpClient.onPacket([](AsyncUDPPacket packet) {
//             // reveive packet check what token is 
//             wsTextPrintf("Received %u bytes from %s, Port %d\n", packet.length(), packet.remoteIP().toString().c_str(), packet.remotePort());

//             for(int i=0;i<packet.length();i++)
//               wsTextPrintf("[%2x]",packet.data()[i]);
            
//             wsTextPrintf("\n");
//         });
//         return true;
//     }
//     return false;
// }


// Block Mode
int udp_Send_Ack(AsyncUDPPacket packet) { //sent ack to remote ip
     return 0;
    int offset = 0;
    uint8_t buffer[PACKETHEADER+4];
    PacketHeader *packet_header = (PacketHeader*)packet.data();
    PacketHeader header = {"ACK"};
    header.seqno =  packet_header->seqno;
    header.from = mpi_comm_rank;   // NOTICE for unconfig this rank is invalide
    header.to =  packet_header->from;
    MPI_DBG_UDP_PRINTF("[ACK]to IP: %s, seqno=%d, from %d to %d\n", packet.remoteIP().toString().c_str(), \
                            packet_header->seqno, packet_header->from, packet_header->to);
    // header.to = toRank;
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);
    memcpy(buffer + offset, &mpi_comm_rank, sizeof(uint32_t)); 
    offset+=sizeof(uint32_t); 
    udp_dump_packet("[UDP][TX]ACK",buffer, offset);
    return udpClient.writeTo(buffer, offset, packet.remoteIP(), listenPort); // Use  
}
void udp_Clear_Ack(int rank) {
    if (rank==-1) {  // check all ack 
        int acks = 0;
        for (int i=0;i<mpi_comm_size;i++) {
            udpmutex.lock();
            mpinode[i].status &= ~MPI_NODE_STATE_ACK;
            // if (mpinode[idx].ip ==  WifiLocalIP() ) {
            //     mpinode[idx].status |= MPI_NODE_STATUS_ACK; // sender no ack
            // }
            udpmutex.unlock();
        }
    }else{ // check dedicate rank
        if (rank<0||rank>=mpi_comm_size) return;
        mpinode[rank].status &= ~MPI_NODE_STATE_ACK;
    }
}
#define UDPTIMEOUT 5000
int udp_Wait_Ack(int rank, int setStatusBit) {
    unsigned long lastTime = millis();
    unsigned long  currentTime = millis();
    while ((currentTime - lastTime) < UDPTIMEOUT) {
        if (rank==-1) {  // check all ack 
            int acks = 0;
            return mpi_comm_size-1;
            for (int i=0;i<mpi_comm_size;i++) {
                udpmutex.lock();
                if (mpinode[i].status & MPI_NODE_STATE_ACK) {
                    acks++;
                    mpinode[i].status |= setStatusBit;
                    MPI_DBG_UDP_PRINTF("[udp_Wait_Ack](rank:%d,status:%x)\n",i,mpinode[i].status); 
                }
                udpmutex.unlock();
            }
            if (acks==mpi_comm_size-1) return acks; // all except sender 
        }else{ // check dedicate rank
            return 1;
            udpmutex.lock();
            if (rank<0||rank>=mpi_comm_size) return 1;
            if (mpinode[rank].status & MPI_NODE_STATE_ACK) {
                mpinode[rank].status |= setStatusBit;
                MPI_DBG_UDP_PRINTF("[udp_Wait_Ack](rank:%d,status:%x)\n",rank,mpinode[rank].status); 
                return 1; //ack done
            }
            udpmutex.unlock();
        }
        delay(100);
        currentTime = millis();
    }
    return -1; // timeout
}

/**
   @brief Block mode Sent, the datapacket is sent then wait acks until timeout
   @details
   This function takes two integers as input and returns their sum.
   
   @param[in] rank -1 is broadcast
                   >0  is single and specific node rank. (must be less than mpi_comm_size)
   @param[in] data   packet
   @param[in] length  packet length
   @param[in] statusBits  state bit, when config it will set to 0x8000 MSB turn on in root
   @return acks: for sucessful, acks cnt, else 0
 */
#define MAX_RETRIES 5
static uint8_t udp_seq=0;
size_t udp_Block_Sent(int rank, uint8_t* data, int length, int statusBits) {
    MPI_DBG_UDP_PRINTF("[udp_Block_Sent](rank:%d,length:%d)\n",rank,length); 
    if (rank ==  mpi_comm_rank) { // localhost
        return 0;
    }
    size_t  n = 0;
    // header
    PacketHeader *header = (PacketHeader*)data;
    header->seqno = udp_seq++;
    header->from = (uint8_t)mpi_comm_rank;
    header->to = rank;
    udp_Clear_Ack(rank);
    for (int retries = 0; retries < MAX_RETRIES; retries++) {
        PacketHeader *header = (PacketHeader *)data;
        header->trycnt = retries;
        if (rank < 0) {// brodacast 
            IPAddress broadcastIP(255,255,255,255);// = ~WiFi.subnetMask() | WiFi.gatewayIP();
            n = udpClient.broadcastTo(data, length, listenPort); // Use  
    MPI_DBG_UDP_PRINTF("[udp_Block_Sent]broadcastTo,(rank:%d,length:%d),listen%d\n",rank,length,listenPort); 
            if (udp_Wait_Ack(rank,statusBits) == mpi_comm_size-1) return n; // all nodes except sender acks back ;
        } else {
            if (rank >= mpi_comm_size) return 0;
            n = udpClient.writeTo(data, length, mpinode[rank].ip, listenPort); // Use  
            if (udp_Wait_Ack(rank,statusBits) == 1) return n; // dedicate ack got
        }
        // fail retry
    }
    MPI_DBG_ERROR_PRINTF("[udp_Block_Sent](rank:%d,length:%d) Fail\n",rank,length); 

    return 0;
}

// nonBlock Mode
size_t udp_Nonblock_Sent(int rank, uint8_t* data, int length, int statusBits) {
    return 0;

}

/*---------------------------------------------------------------------------*/
/* MPI_MSG layer, which format and decode Packet                             */
/*---------------------------------------------------------------------------*/



// _ private IPAddress convert
String _formatIPAddress(IPAddress ipAddress) {
  String formattedIP;
  for (int i = 0; i < 4; i++) {
    if (i > 0) {
      formattedIP += ".";
    }
    formattedIP += String(ipAddress[i], DEC);
    if (ipAddress[i] < 10) {
      formattedIP += "00";
    } else if (ipAddress[i] < 100) {
      formattedIP += "0";
    }
  }
  return formattedIP;
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

/*---------------------------------------------------------------------------*/
/* udp tx rx handler and packet message dispatch                             */
/*---------------------------------------------------------------------------*/

bool MPI_MSG_Ack_Or_Not(AsyncUDPPacket packet){
    // Determine packet type
    int offset = 0;
    PacketHeader *packet_header = (PacketHeader*)packet.data();
    if (strncmp(packet_header->ptype, "ACK", PACKET_TYPE_SIZE) != 0){ // not ACK
        return false;
    }
    udp_Send_Ack(packet);
    return true;

}

// TODO hash header.ptye to indirect call dispatch function
bool MPI_MSG_Packet_Dispatch(uint8_t *buffer, int len){
    // Determine packet type
    int offset = 0;
    PacketHeader header;
    memcpy(&header, buffer, sizeof(PacketHeader)); offset+=sizeof(PacketHeader);
    header.ptype[PACKET_TYPE_SIZE-1] = 0;
    MPI_DBG_MSG_PRINTF("[RX]dispatch(%s)\n",header.ptype);

    if (strncmp(header.ptype, "CFG", PACKET_TYPE_SIZE) == 0) {
        // Parse the configuration
        MPI_MSG_Config_Parser(buffer, len);
    } else if (strncmp(header.ptype, "EXE", PACKET_TYPE_SIZE) == 0) {
        // Handle the "EXE" packet, run example at each node
        int example = 0;
        memcpy(&example, buffer+offset, sizeof(int)); offset+=sizeof(int);
        MPI_DBG_MSG_PRINTF("(rank:%d) example(%x)\n",mpi_comm_rank,example); 
        wsSerial.push("mhello\n",7); //websocket input
        // _MPI_Run_Example(example);
        // Implement logic for "Run" packet
    } else if (strncmp(header.ptype, "ACK", PACKET_TYPE_SIZE) == 0) {
        // Handle the "ACK" packet
        int remoteRank;
        memcpy(&remoteRank, buffer+offset, sizeof(uint32_t)); offset+=sizeof(uint32_t);
        if (remoteRank<0||remoteRank>=mpi_comm_size) return false;
        udpmutex.lock();
        mpinode[remoteRank].status|=MPI_NODE_STATE_ACK;
        udpmutex.unlock();
        MPI_DBG_UDP_PRINTF("(from rank:%x)\n",remoteRank);
        return false;
        // Implement logic for "Run" packet
    } else if (strncmp(header.ptype, "PRT", PACKET_TYPE_SIZE) == 0) {
        MPI_PRINTF((const char *)(buffer+offset)); // redirect IO to root
    } else if (strncmp(header.ptype, "LED", PACKET_TYPE_SIZE) == 0) {
        uint16_t onoff;
        memcpy(&onoff, buffer+offset, sizeof(int)); offset+=sizeof(int);
        MPI_DBG_MSG_PRINTF("(rank:%d) packet(%x)\n",mpi_comm_rank,onoff); 
        pwmled(onoff);
    } else if (strncmp(header.ptype, "RST", PACKET_TYPE_SIZE) == 0) {
        // Handle the "EXEC" packet
        // Implement logic for "Run" packet
        pwmled(0);
        ESP.restart();
    } else {
        return false;
    }
    return true; // for ack
}

// if < 0 fail timeout 
int MPI_MSG_Sent_LED(int rank,int onoff) { //turn on led on/off
    MPI_DBG_MSG_PRINTF("[LED](rank:%d,onoff:%d)\n",rank,onoff); 
    int offset = 0;
    uint8_t buffer[PACKETHEADER+4];
    PacketHeader header = {"LED"};
    header.from = mpi_comm_rank;
    header.to = rank;
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);
    memcpy(buffer + offset, &onoff, sizeof(int)); 
    offset+=sizeof(int);

    if (rank==-1||rank==mpi_comm_rank) {
        MPI_MSG_Packet_Dispatch(buffer, offset); // LED active at localhost
        return MPI_SUCCESS;
    }

    udp_dump_packet("[MSG][TX]LED",buffer, offset);
    int n = udp_Block_Sent(rank, buffer, offset,0); // Use  
    return MPI_SUCCESS;
}
int MPI_MSG_Sent_RST(int rank) { // reboot rabk
    MPI_DBG_MSG_PRINTF("[RST](rank:%d)",rank); 
    int offset = 0;
    uint8_t buffer[PACKETHEADER+4];
    PacketHeader header = {"RST"};
    header.from = mpi_comm_rank;
    header.to = rank;
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);
    memcpy(buffer + offset, &rank, sizeof(int)); 
    offset+=sizeof(int);
    int n = udp_Block_Sent(rank, buffer, offset,0); // sent RST to others node 
    if (rank==-1||rank==mpi_comm_rank) {
        delay(5000);
        MPI_MSG_Packet_Dispatch(buffer, offset); // LED active at localhost
    } 
    return MPI_SUCCESS;

}
int MPI_MSG_Sent_EXE(int example) { // run on all node
    MPI_DBG_MSG_PRINTF("[EXE](rank:%d,example:%d)\n",-1,example); 
    int offset = 0;
    uint8_t buffer[PACKETHEADER+4];
    PacketHeader header = {"EXE"};
    header.from = mpi_comm_rank;
    header.to = -1;
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);
    memcpy(buffer + offset, &example, sizeof(int)); 
    offset+=sizeof(int);   
    int n = udp_Block_Sent(-1, buffer, offset,0); // sent EXE to others node 
    MPI_MSG_Packet_Dispatch(buffer, offset); // RUN active at localhost
    return MPI_SUCCESS;

}

/*---------------------------------------------------------------------------*/
/* mdns_ LAN Discovery to build Config Table                                 */
/* udp_ send receive message layer                                           */
/*---------------------------------------------------------------------------*/
int MPI_MSG_Scan_And_Config(){ 
  int n=WiFimDNSQuery();
  int esp32nodes = 0;
    esp32nodes++;

  if (n == 0) {
        MPI_DBG_WARNING_PRINTF("[mDNS] scan No others services found\n");
  } else {
    for (int i = 0; i < n; ++i) {
      if (String(MDNS.hostname(i).substring(0,11)) == String(WIFIAPNAME)) {
        esp32nodes++;
      }
    }
  }
  // wsTextPrintf("%d mdnsQuery found and %d services found\n",n,esp32nodes);
  // alloc mpinode to store 
  if (mpinode!=NULL) free(mpinode);
  mpinode = (MPI_NODE*) malloc(sizeof(MPI_NODE)*esp32nodes);
  if (mpinode == NULL) return -1;
  int idx=0;
    strncpy(mpinode[idx].hostname, WifimDNSName().c_str(),HOSTNAMESIZE-1);  mpinode[idx].hostname[HOSTNAMESIZE-1]=0;
    // strncpy(mpinode[idx].ip, _formatIPAddress(WiFi.localIP()).c_str(),IPV4SIZE-1);  mpinode[idx].ip[IPV4SIZE-1]=0;
    mpinode[idx].ip = WiFi.localIP(); 
    mpinode[idx].status = MPI_NODE_STATE_INIT;
    // wsTextPrintf("%2d:%04x:",idx,mpinode[idx].status); 
    // wsTextPrintf("%s:",mpinode[idx].hostname);
    // wsTextPrintf("%s\n",mpinode[idx].ip.toString().c_str());
    idx++;
  if (esp32nodes == 1) {
  } else {
    for (int i = 0; i < n; ++i) {
      if (String(MDNS.hostname(i).substring(0,11)) == String(WIFIAPNAME)) {
        strncpy(mpinode[idx].hostname, MDNS.hostname(i).c_str(),HOSTNAMESIZE-1);  mpinode[idx].hostname[HOSTNAMESIZE-1]=0;
        mpinode[idx].ip = MDNS.IP(i); 
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
  _mpi_state=MPI_NODE_STATE_CONFIG|MPI_NODE_STATE_ROOT;
  _MPI_Config_Dump();

  // config
  MPI_MSG_Config_Broardcast(mpinode, mpi_comm_size);
  return MPI_SUCCESS;
}

bool MPI_MSG_Config_Broardcast( MPI_NODE nodes[], int nodeCount) {
    int offset = 0;
    MPI_DBG_MSG_PRINTF("MPI_MSG_Config_Broardcast(size:%d)\n",nodeCount);
    unsigned char *buffer= (unsigned char *)malloc(40*nodeCount);
    if (buffer==NULL) return false;

    PacketHeader header = {"CFG"};
    header.from = 0;
    header.to = 0xff; // broadcast
    
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);
    memcpy(buffer + offset, &nodeCount, sizeof(int));  
    offset+=sizeof(int);
    for (int i = 0; i < nodeCount; i++) {
        nodes[i].status = 0;
        memcpy(buffer + offset, &i, sizeof(int)); // index as rank
        offset += sizeof(int);
        memcpy(buffer + offset, nodes[i].hostname, HOSTNAMESIZE);
        offset +=  HOSTNAMESIZE;
        String fmtip =  _formatIPAddress(nodes[i].ip);
        memcpy(buffer + offset, fmtip.c_str(), IPV4SIZE);
        offset += IPV4SIZE;
    }
    if (udp_Block_Sent(-1,buffer,offset,0)==0)  return true;

    return false;
}

int MPI_MSG_Config_Parser(uint8_t* data, int dataSize) {
    int offset = sizeof(PacketHeader);
    int esp32nodes;
    memcpy(&esp32nodes, data + offset, sizeof(int));
    mpi_comm_size = esp32nodes; // MPI_size
    offset+=sizeof(int);   
    if (esp32nodes<0||esp32nodes>MAX_NODES) return -1;
    // alloc mpinode to store 
    if (mpinode!=NULL) free(mpinode); // free old 
    mpinode = (MPI_NODE *) malloc(sizeof(MPI_NODE)*esp32nodes);
    if (mpinode == NULL) return -1;
    String consoleStr = "ESPCONSOLE-XXXX";
    String fmtip = "192.168.001.001";
    for(int i=0;i<esp32nodes;i++){
        int rank;
        char ip[IPV4SIZE]; ip[IPV4SIZE-1]=0;
        memcpy(&rank, data + offset, sizeof(int));
        offset += sizeof(int);
        mpinode[i].status = 0;
        memcpy(mpinode[i].hostname, data + offset, HOSTNAMESIZE); mpinode[i].hostname[HOSTNAMESIZE-1]=0;
        offset +=  HOSTNAMESIZE;
        memcpy(ip, data + offset, IPV4SIZE);
        offset += IPV4SIZE;
        mpinode[i].ip = _parseIPAddress(String(ip));
        if (String(mpinode[i].hostname) == WifimDNSName()) {
            mpi_comm_rank = i; // MPI_rank assign rank
        }
        if (mpinode[i].ip == WiFi.localIP()) {
            mpi_comm_rank = i; // MPI_rank assign rank
        }
    }
    // MPI_DBG_MSG_PRINTF("[udp_Config_Parser](size:%d,rank:%d)\n",mpi_comm_size,mpi_comm_rank);
    _mpi_state=MPI_NODE_STATE_CONFIG; // not root
    _MPI_Config_Dump();

    return mpi_comm_size;
}


/*---------------------------------------------------------------------------*/
/* _MPI_API private                                                          */
/*---------------------------------------------------------------------------*/
int _MPI_Config_Dump() {
    if (_mpi_state==0) {
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
// static link and execute example
int cmd_mpi_hello(int argc,char *argv[]);
int _MPI_Run_Example(int example) {
    int argc = 1;
    char *argv[]={(char *)"mhello"};
    if (mpinode==NULL ) { //unconfig 
        return -1;
    }
    switch(example){
    case 0:
    default:
        cmd_mpi_hello(argc,argv);
        break;
    }
    return MPI_SUCCESS;
}

#include <string.h>
// API and also MSG layers
void MPI_printf(const char *fmt,...){
  int sizebuf = strlen(fmt)+100;
  char *buffer=(char *)malloc(sizebuf);
  int offset = 0;
    PacketHeader header = {"PRT"};
    header.from = mpi_comm_rank;
    header.to = 0;
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);  
  va_list args;
  va_start(args, fmt);
  int length = vsnprintf(buffer+offset, sizebuf, fmt, args); offset+=length;
  int n=0;
  va_end(args);  
  if (mpi_comm_rank==0 ){
        MPI_MSG_Packet_Dispatch((uint8_t*)buffer, offset); // RUN active at localhost

  }else{
        n = udp_Block_Sent(-1,(uint8_t*)buffer,offset,0); // extension 
  }
  MPI_DBG_MSG_PRINTF("[MPI_printf](%d,%d)\n",offset,n);
  free(buffer);

}

/*---------------------------------------------------------------------------*/
/* MPI_API subset                                                            */
/*---------------------------------------------------------------------------*/
int MPI_Init(int *argc, char ***argv) {
    mpi_initialized = 0;
    // Normally, you would parse command-line arguments and set up the environment here.
    int ret = MPI_SUCCESS;
    if (_mpi_state==0) { // force config
        ret = MPI_MSG_Scan_And_Config();
    } 
    if (ret != MPI_SUCCESS) return ret;
    mpi_initialized = 1;
    MPI_DBG_API_PRINTF("[MPI_Init](%d)\n",*argc);
    return MPI_SUCCESS;
}
int MPI_Finalize(void) {
    mpi_initialized = 0;
    MPI_DBG_API_PRINTF("[MPI_Finalize]()\n");
    // if (mpinode) free(mpinode);
    // mpi_comm_size = 0;
    // mpi_comm_rank = -1;
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
    // sprintf(name,"NODE%2d",mpi_comm_rank);
    memcpy(name,mpinode[mpi_comm_rank].hostname,HOSTNAMESIZE);
    *resultlen = HOSTNAMESIZE-1;
    MPI_DBG_API_PRINTF("[MPI_Send]MPI_Get_processor_name(%s,%d)\n",name,*resultlen);
    return MPI_SUCCESS;
}


int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
    if (!mpi_initialized) return -1;
    // Simplified send: In a real implementation, you'd use network communication
    MPI_DBG_API_PRINTF("[MPI_Send] %d items to process %d\n", count, dest);
    return MPI_SUCCESS;
}

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status) {
    if (!mpi_initialized) return -1;
    // Simplified receive: In a real implementation, you'd use network communication
    MPI_DBG_API_PRINTF("[MPI_Recv]Receiving %d items from process %d\n", count, source);
    return MPI_SUCCESS;
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


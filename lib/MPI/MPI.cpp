// TODO hash header.ptye to indirect call packet udp dispatch function
#include "MPI_lw.h"            // TODO port as arduino library 
// #include "wifiinit.h"       // TODO wifi mdns function should be move here 




// ***********UDP**************
AsyncUDP udpServer;
AsyncUDP udpClient;
const uint16_t listenPort = 8987;

// ***********PACKET MESSAGE**************
// udp packet header
#define PACKET_TYPE_SIZE 4
// CFG
// RUN
// ACK rank   rank respone ack
// LED rank 0|1   turn on off led[rank]
// RST rank       reboot
// PRT MESSAGE    printf
struct PacketHeader {
    char pmsg[PACKET_TYPE_SIZE];
    uint8_t seqno;
    uint8_t from;
    uint8_t to;  // signed byte
    uint8_t trycnt;
};
static uint8_t _udp_send_seq=0;
static uint8_t _udp_arrive_seq=0;
#define PACKETHEADER 8 // pcket headersize if PacketHeader change, modify it



// MPINODE STATUS
#define MPI_NODE_STATE_INIT   0x0000 // root scan all nodes
#define MPI_NODE_STATE_CONFIG 0x0001 // config done, rank is assigned
#define MPI_NODE_STATE_ROOT   0x0002 // config done,as root
#define MPI_NODE_STATE_ACK    0x8000 // ack bit
static int mpi_initialized = 0;
int _mpi_state = 0;
MPI_NODE *mpinode=NULL;
std::mutex udpmutex,upd_seq_mutex,upd_dispatch_mutex;
static uint32_t mpi_comm_size = 0;
#define RANK_INIT 0 // initial config for node
static uint32_t mpi_comm_rank = RANK_INIT;

uint16_t MPI_debugLevel =  MPI_DBG_ERROR  | MPI_DBG_WARNING | MPI_DBG_PINGPONG;// |MPI_DBG_MSG;//| MPI_DBG_UDP ;//| MPI_DBG_INFO  | MPI_DBG_MSG ;//| MPI_DBG_UDP| MPI_DBG_MSG| MPI_DBG_API; // Enable error and warning messages



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


void udpserver_onPacket(AsyncUDPPacket packet) {
    // Determine packet type, check ack, nak, pin without ack reply
    #define ACK_FLAG (1<<0)
    #define PIN_FLAG (2<<0)
    uint8_t msg_Flag = ACK_FLAG;
    PacketHeader *header = (PacketHeader *)packet.data();
    MPI_DBG_UDP_PRINTF("[RX]onPacket(%s):[%d]frmIP:%s,from(%d,%d)\n", \
            header->pmsg,header->seqno,packet.remoteIP().toString().c_str(),header->from,header->to);
    if ((strncmp(header->pmsg, "ACK", PACKET_TYPE_SIZE) == 0) || \
            (strncmp(header->pmsg, "PON", PACKET_TYPE_SIZE) == 0)){
        upd_seq_mutex.lock();
        _udp_arrive_seq = header->seqno;
        upd_seq_mutex.unlock();
        msg_Flag &= ~ ACK_FLAG;
    }else if (strncmp(header->pmsg, "NAK", PACKET_TYPE_SIZE) == 0) {
        msg_Flag &= ~ ACK_FLAG;
    }else if (strncmp(header->pmsg, "PIN", PACKET_TYPE_SIZE) == 0){
        msg_Flag &= ~ ACK_FLAG;
        msg_Flag |=  PIN_FLAG;
    }
    if (msg_Flag & ACK_FLAG) { // ACK respone
        PacketHeader *packet_header = (PacketHeader *)packet.data();
        PacketHeader header = {"ACK"};
        header.seqno =  packet_header->seqno;
        header.from = mpi_comm_rank;   // NOTICE for unconfig this rank is invalide
        header.to =  packet_header->from;
        udpClient.writeTo((uint8_t*)&header, sizeof(PacketHeader), packet.remoteIP(), listenPort); // Use  
        MPI_DBG_UDP_PRINTF("[UDP]onPacket()ACK reply remoteIP:%s, from_to(%d,%d)\n", \
                 packet.remoteIP().toString().c_str(), mpi_comm_rank,packet_header->from);
    }else if (msg_Flag & PIN_FLAG){ // PONG respone
        PacketHeader *packet_header = (PacketHeader *)packet.data();
        PacketHeader header = {"PON"};
        header.seqno =  packet_header->seqno;
        header.from = mpi_comm_rank;   // NOTICE for unconfig this rank is invalide
        header.to =  packet_header->from;
        udpClient.writeTo((uint8_t*)&header, sizeof(PacketHeader), packet.remoteIP(), listenPort); // Use    
        delay(100); // to avoid collision for MPI_DBG_MSG_PRINTF
        MPI_DBG_MSG_PRINTF("[UDP]onPacket()PON reply remoteIP:%s, from_to(%d,%d)\n", \
                 packet.remoteIP().toString().c_str(), mpi_comm_rank,packet_header->from);
        return;  
    }
    upd_dispatch_mutex.lock(); // avoid reentry
    MPI_MSG_Packet_Dispatch(packet.data(), packet.length()); // process packet
    upd_dispatch_mutex.unlock(); // avoid reentry
    // udp_dump_packet("\n[UDP][RX]onPacket()Dump",packet.data(), packet.length());
}
// udp server handle 
bool udpserver_init() {
    if (!WiFimDNS_init()) {
        MPI_DBG_ERROR_PRINTF("udpserver_init():WiFimDNS_init Fail\n");
        return false;
    }
 // Setup UDP Server
    if (udpServer.listen(listenPort)) {
        MPI_DBG_UDP_PRINTF("udpserver_init():Listening on IP: %s, Port: %d\n", WiFi.localIP().toString().c_str(), listenPort);
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
// int udp_Send_Ack(uint8_t* data, int length) { // incoming packet data and length
//      return 0;
//     int offset = 0;
//     uint8_t buffer[PACKETHEADER+4];
//     PacketHeader *packet_header = (PacketHeader *)data;
//     PacketHeader header = {"ACK"};
//     header.seqno =  packet_header->seqno;
//     header.from = mpi_comm_rank;   // NOTICE for unconfig this rank is invalide
//     header.to =  packet_header->from;
//     MPI_DBG_UDP_PRINTF("[ACK]to IP: %s, seqno=%d, from %d to %d\n", mpinode[header.to ].ip.toString().c_str(), \
//                             packet_header->seqno, packet_header->from, packet_header->to);
//     // header.to = toRank;
//     memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
//     offset+=sizeof(PacketHeader);
//     memcpy(buffer + offset, &mpi_comm_rank, sizeof(uint32_t)); 
//     offset+=sizeof(uint32_t); 
//     udp_dump_packet("[UDP][TX]ACK",buffer, offset);
//     return udpClient.writeTo(buffer, offset, mpinode[header.to ].ip, listenPort); // Use  
// }
void udp_Clear_Ack(int rank) {
    if (mpinode==NULL || mpi_comm_size == 0) return;
    if (rank == BROADCAST_RANK) {  // check all ack 
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
        udpmutex.lock();
        mpinode[rank].status &= ~MPI_NODE_STATE_ACK;
        udpmutex.unlock();

    }
}
#define UDPTIMEOUT 5000
int udp_Wait_Ack(int rank, int setStatusBit) {
    unsigned long lastTime = millis();
    unsigned long  currentTime = millis();
    if (mpinode==NULL || mpi_comm_size == 0) return 0; // unconfig
    if (_mpi_state == 0) { // unconfig
        return (rank == BROADCAST_RANK)  ? mpi_comm_size:1;
    }
    while ((currentTime - lastTime) < UDPTIMEOUT) {
        if (rank==BROADCAST_RANK) {  // check all ack 
            int acks = 1; // for localhost
            // return mpi_comm_size-1; // TODO ****DEBUG
            for (int i=0;i<mpi_comm_size;i++) {
                udpmutex.lock();
                if (mpinode[i].status & MPI_NODE_STATE_ACK) {
                    acks++;
                    mpinode[i].status |= setStatusBit;
                    MPI_DBG_UDP_PRINTF("[udp_Wait_Ack](rank:%d,status:%x)\n",i,mpinode[i].status); 
                }
                udpmutex.unlock();
            }
            if (acks==mpi_comm_size) return acks; // all except sender 
        }else{ // check dedicate rank
            // return 1; // TODO ****DEBUG
            if (rank<0||rank>=mpi_comm_size||rank==mpi_comm_rank) return 1; // localhost
            udpmutex.lock();
            int ackbit = mpinode[rank].status & MPI_NODE_STATE_ACK;
            udpmutex.unlock();
            if (ackbit) {
                udpmutex.lock();
                mpinode[rank].status |= setStatusBit;
                MPI_DBG_UDP_PRINTF("[udp_Wait_Ack](rank:%d,status:%x)\n",rank,mpinode[rank].status); 
                udpmutex.unlock();
                return 1; //ack done
            }
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
   
   @param[in] rank BROADCAST_RANK is broadcast
                   >0  is single and specific node rank. (must be less than mpi_comm_size)
   @param[in] data   packet
   @param[in] length  packet length
   @param[in] statusBits  state bit, when config it will set to 0x8000 MSB turn on in root
   @return acks: for sucessful, acks cnt, else 0
 */
#define MAX_RETRIES 5

size_t udp_Block_Send(uint8_t* data, int length) {
    MPI_DBG_MSG_PRINTF("udp_Block_Send()\n");
    size_t  n = 0;
    // header
    PacketHeader *header = (PacketHeader*)data;
    upd_seq_mutex.lock();
    header->seqno = _udp_send_seq+1; _udp_send_seq++;
    upd_seq_mutex.unlock();
    header->from = (uint8_t)mpi_comm_rank;
    uint8_t rank  = header->to; 
    if (_mpi_state == 0) { // unconfig
        rank = BROADCAST_RANK; // unknow ip redirect to broadcast
    }
    MPI_DBG_MSG_PRINTF("%s(rank:%d[%d]from(%d,%d)\n",header->pmsg,rank,header->seqno,header->from,header->to); 
    udp_Clear_Ack(rank);
    int acks = 0;
    if (rank == BROADCAST_RANK || rank == mpi_comm_rank) {// brodacast 
        upd_dispatch_mutex.lock(); // avoid reentry
        MPI_MSG_Packet_Dispatch(data, length); 
        upd_dispatch_mutex.unlock();

        udp_dump_packet("\n[UDP][RX]udp_Block_Send()Dump",data, length);
        if (rank != BROADCAST_RANK ) return length; //localhost non broadcast
    }
    for (int retries = 0; retries < MAX_RETRIES; retries++) {
        PacketHeader *header = (PacketHeader *)data;
        header->trycnt = retries;
        if (rank == BROADCAST_RANK) {// brodacast 
            acks++; // localhost
            IPAddress broadcastIP(255,255,255,255);// = ~WiFi.subnetMask() | WiFi.gatewayIP();
            n = udpClient.broadcastTo(data, length, listenPort); // Use  
            MPI_DBG_MSG_PRINTF("[udp_Block_Send]broadcastTo,(rank:%d, retries%d,length:%d)\n",rank,retries,length); 
            acks += udp_Wait_Ack(rank,0);
            if ( acks >= mpi_comm_size){ // consider unconfig mpi_comm_size == 0
                MPI_DBG_MSG_PRINTF("[udp_Block_Send]broadcastTo Ack Done,(rank:%d,%d),retries:%d)\n",rank,n,retries); 
                return n; // all nodes except sender acks back ;
            } 
        } else {
            if (rank<0 || rank >= mpi_comm_size) return 0;
            MPI_DBG_MSG_PRINTF("[udp_Block_Send]writeTo,(rank:%d,,retries%dlength:%d),ip%s\n",rank,retries,length, mpinode[rank].ip.toString().c_str()); 
            n = udpClient.writeTo(data, length, mpinode[rank].ip, listenPort); // Use  
            if (udp_Wait_Ack(rank,0) == 1) {
                MPI_DBG_MSG_PRINTF("[udp_Block_Send]writeTo Ack Done,(rank:%d,%d),retries:%d)\n",rank,n,retries); 
                return n; // dedicate ack got
            }
        }
        // fail retry
    }
    MPI_DBG_ERROR_PRINTF("[udp_Block_Send](rank:%d,length:%d) Fail\n",rank,length); 

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
  char buffer[20];
  sprintf(buffer,"%03d.%03d.%03d.%03d",ipAddress[0],ipAddress[1],ipAddress[2],ipAddress[3]);
  MPI_DBG_MSG_PRINTF("_formatIPAddress(%s)%03d.%03d.%03d.%03d\n",buffer,ipAddress[0],ipAddress[1],ipAddress[2],ipAddress[3]);
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

// bool MPI_MSG_Ack_Or_Not(AsyncUDPPacket packet){
//     // Determine packet type
//     int offset = 0;
//     PacketHeader *packet_header = (PacketHeader*)packet.data();
//     if (strncmp(packet_header->pmsg, "ACK", PACKET_TYPE_SIZE) != 0){ // not ACK
//         return false;
//     }
//     udp_Send_Ack(packet.data(), packet.length());
//     return true;

// }

typedef struct {
    uint32_t        typever;
    MPI_Datatype    typecode;
    short        typesize;
    short        typecount;
    short        typereserved;
    uint8_t         typespace[0];
}TYPEARRAY;


// TODO hash header.ptye to indirect call dispatch function
bool MPI_MSG_Packet_Dispatch(uint8_t *buffer, int len){
    // Determine packet type
    int offset = 0;
    PacketHeader header;
    memcpy(&header, buffer, sizeof(PacketHeader)); offset+=sizeof(PacketHeader);
    header.pmsg[PACKET_TYPE_SIZE-1] = 0;
    MPI_DBG_MSG_PRINTF("[RX]dispatch(%s)[%d](%d,%d)\n",header.pmsg,header.seqno,header.from,header.to);

    if (strncmp(header.pmsg, "CFG", PACKET_TYPE_SIZE) == 0) {
        // Parse the configuration
        MPI_MSG_Config_Parser(buffer, len);
    } else if (strncmp(header.pmsg, "RUN", PACKET_TYPE_SIZE) == 0) {
        // Handle the "RUN" packet, run example at each node
        int example = 0; // TODO example should be assign when examples are prepared
           buffer[len-1]=0;
        String cmd = String((const char *)(buffer+offset));
        cmd += "\n";
        // memcpy(&example, buffer+offset, sizeof(int)); offset+=sizeof(int);
        // if (example>MPI_TUTORIALS) example = 0;
        MPI_DBG_MSG_PRINTF("(rank:%d) example(%s)\n",mpi_comm_rank,cmd.c_str()); 
        MPI_Iot_MPICH(cmd.c_str());


        // _MPI_Run_Example(example);
        // Implement logic for "Run" packet
    } else if ((strncmp(header.pmsg, "ACK", PACKET_TYPE_SIZE) == 0) || \
               (strncmp(header.pmsg, "PON", PACKET_TYPE_SIZE) == 0)) {
        // Handle the "ACK" packet
        int remoteRank = header.from;
        // memcpy(&remoteRank, buffer+offset, sizeof(uint32_t)); offset+=sizeof(uint32_t);
        if (mpinode != NULL && remoteRank < mpi_comm_size) {
            udpmutex.lock();
            mpinode[remoteRank].status|=MPI_NODE_STATE_ACK;
            udpmutex.unlock();
        }
        MPI_DBG_UDP_PRINTF("(from rank:%x)\n",remoteRank);
        return false;
        // Implement logic for "Run" packet
    } else if (strncmp(header.pmsg, "PRT", PACKET_TYPE_SIZE) == 0) {
        buffer[len-1]=0;
        MPI_PRINTF((const char *)(buffer+offset)); // redirect IO to root
    } else if (strncmp(header.pmsg, "LED", PACKET_TYPE_SIZE) == 0) {
        int onoff;
        memcpy(&onoff, buffer+offset, sizeof(int)); offset+=sizeof(int);
        MPI_DBG_MSG_PRINTF("(rank:%d) onoff(%x)\n",mpi_comm_rank,onoff); 
        MPI_Iot_LED(onoff);
    } else if (strncmp(header.pmsg, "DBG", PACKET_TYPE_SIZE) == 0) {
        int debug;
        memcpy(&debug, buffer+offset, sizeof(int)); offset+=sizeof(int);
        MPI_DBG_MSG_PRINTF("(rank:%d) debug(%x)\n",mpi_comm_rank,debug); 
        MPI_debugLevel = debug;    
    } else if (strncmp(header.pmsg, "STM", PACKET_TYPE_SIZE) == 0) {
        unsigned long epoch;
        memcpy(&epoch, buffer+offset, sizeof(unsigned long)); offset+=sizeof(unsigned long);
        MPI_DBG_MSG_PRINTF("(rank:%d) epoch(%ul)\n",mpi_comm_rank,epoch); 
        MPI_Iot_SetEpoch(epoch);    
    } else if (strncmp(header.pmsg, "RST", PACKET_TYPE_SIZE) == 0) {
        // Handle the "RST" packet
        uint32_t timeout;
        memcpy(&timeout, buffer+offset, sizeof(int)); offset+=sizeof(int);       
        MPI_Iot_Restart(timeout);
    } else {
        return false;
    }
    return true; // for ack
}
// if < 0 fail timeout 
int MPI_MSG_Sent_DEBUG(int rank,int debug) { //turn on debug mode 
    MPI_DBG_MSG_PRINTF("[LED](rank:%d,debug:%d)\n",rank,debug); 
    return MPI_MSG_Sent_CMD("DBG",rank,debug);
}
// if < 0 fail timeout 
int MPI_MSG_Sent_LED(int rank,int onoff) { //turn on led on/off
    MPI_DBG_MSG_PRINTF("[LED](rank:%d,onoff:%d)\n",rank,onoff); 
    return MPI_MSG_Sent_CMD("LED",rank,onoff);
    // int offset = 0;
    // uint8_t buffer[PACKETHEADER+4];
    // PacketHeader header = {"LED"};
    // header.from = mpi_comm_rank;
    // header.to = rank;
    // memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    // offset+=sizeof(PacketHeader);
    // memcpy(buffer + offset, &onoff, sizeof(int)); 
    // offset+=sizeof(int);
// 
    // if (rank==BROADCAST_RANK||rank==mpi_comm_rank) {
        // MPI_MSG_Packet_Dispatch(buffer, offset); // LED active at localhost
        // if (rank==mpi_comm_rank) return MPI_SUCCESS;
    // }
// 
    // udp_dump_packet("[MSG][TX]LED",buffer, offset);
    // int n = udp_Block_Send(buffer, offset); // Use  
    // return MPI_SUCCESS;
}
int MPI_MSG_Sent_RST(int rank,int timeout) { // reboot rabk
    MPI_DBG_MSG_PRINTF("[RST](rank:%d) timeout(%d)\n",rank,timeout); 
    return MPI_MSG_Sent_CMD("RST",rank,timeout);
}
int MPI_MSG_Sent_RUN(int rank, int example) { // run on all node
    MPI_DBG_MSG_PRINTF("[RUN](rank:%d,example:%d)\n",rank,example); 
    return MPI_MSG_Sent_CMD("RUN",rank,example);
}
int MPI_MSG_Sent_PIN(int rank) { // ping 
    MPI_DBG_PINGPONG_PRINTF("PING%d\n",rank);
    return MPI_MSG_Sent_CMD("PIN",rank);
}
int MPI_MSG_Sent_PON(int rank) { // pong 
    MPI_DBG_PINGPONG_PRINTF("PONG%d\n",rank);
    return MPI_MSG_Sent_CMD("PON",rank);
}
/*---------------------------------------------------------------------------*/
/* mdns_ LAN Discovery to build Config Table                                 */
/* udp_ send receive message layer                                           */
/*---------------------------------------------------------------------------*/
bool _MPI_MSG_Config_Broardcast( MPI_NODE nodes[], int nodeCount) {
    int offset = 0;
    MPI_DBG_MSG_PRINTF("_MPI_MSG_Config_Broardcast(size:%d)\n",nodeCount);
    unsigned char *buffer= (unsigned char *)malloc(40*nodeCount);
    if (buffer==NULL) return false;

    PacketHeader header = {"CFG"};
    header.from = 0;
    header.to = BROADCAST_RANK; // broadcast
    
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);
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
    if (udp_Block_Send(buffer,offset)==0)  return true;

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
  if (mpinode!=NULL) free(mpinode);
  mpinode = (MPI_NODE*) malloc(sizeof(MPI_NODE)*esp32nodes);
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
//   _MPI_Config_Dump();
  MPI_Iot_LED_Blink(2);
  // config
  _MPI_MSG_Config_Broardcast(mpinode, mpi_comm_size);
  return MPI_SUCCESS;
}
int MPI_MSG_Config_Parser(uint8_t* data, int dataSize) {
    int offset = sizeof(PacketHeader);
    int esp32nodes;
    memcpy(&esp32nodes, data + offset, sizeof(int));
    mpi_comm_size = esp32nodes; // MPI_size
    offset+=sizeof(int);   
    if (esp32nodes<0||esp32nodes>MPI_MAX_NODES) return -1;
    // alloc mpinode to store 
    if (mpinode!=NULL) free(mpinode); // free old 
    mpinode = (MPI_NODE *) malloc(sizeof(MPI_NODE)*esp32nodes);
    if (mpinode == NULL) return -1;
    String consoleStr = "ESPCONSOLE-XXXX";
    String fmtip = "192.168.001.001";
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
    if (mpi_comm_rank==0) MPI_Iot_LED_Blink(mpi_comm_size);

    return mpi_comm_size;
}
#define MESSAGE LENGTH 256
#define MSG_ACTION_NOACK (1<<0)
typedef struct{
    int  size;
    const char *msg;
    const char *fmt;
}MSGFMT;
MSGFMT msgformat[]= {
     8, "CFG", "i" ,     //0 CFG rank(int) 
    12, "DBG", "ii",    //1 DBG rank(int) debugflag(int)
    12, "LED", "ii",    //2 LED rank(int) onoff(int)
    128, "PRT", "is" ,     // RODO i + PRT rank(int) string(char *)
    8, "PIN", "i" ,     //3 PING rank(int) Epoch(unsigned long)
    8, "PON", "i" ,     //4 PONG rank(int) Epoch(unsigned long)
    12, "RST", "ii" ,     //5 RES rank(int) onoff(int)
    128, "RUN", "is",     //6 RUN rank(int) tutorial(int)
    20, "TST", "iiii",    //7 RUN rank(int) tutorial(int)
    12, "STM", "it",    //7 GTM rank(int) Epoch(unsigned long) SET Epoch >>> <<<AXK
    12, "GTM", "it",    //7 STM rank(int) Epoch(unsigned long) GET Epoch >>> <<<ACK Epoch
    12, "ATM", "t",    //7 STM rank(int) Epoch(unsigned long) ACK Epoch 
    20, "ACK", "",    //7 RUN rank(int) tutorial(int)
    -1, ""   , ""  
};
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
#define MPI_TYPE_VER 0x0001

int MPI_MSG_Sent_CMD(const char* XXX, ...) { //send message packet 
    // exception , CFG
    String message = String(XXX);
    if (message==String("CFG")) {
        return MPI_MSG_Scan_And_Config();
    }
    // search fmt table
    int i;
    for(i=1;i<sizeof(msgformat)/sizeof(MSGFMT);i++){
        if (msgformat[i].size<0) return -1; // not found
        if (message == String(msgformat[i].msg)) break;
    }

    // arglist 
    va_list args;
    va_start(args, XXX);

    int rank =  (int)  va_arg(args, int);
    MPI_DBG_MSG_PRINTF("MPI_MSG_Sent_CMD()%s rank:%d ",XXX,rank);

    // found match item, read to parser FMT string to get args
    MSGFMT *pmsg = &msgformat[i];
    const char* fmt=pmsg->fmt; fmt++; //skip rank
    uint8_t *buffer=(uint8_t *)malloc(pmsg->size+sizeof(PacketHeader)-sizeof(int)); // -4, rank in header
    int offset = 0;
    // header 
    PacketHeader header;
    memcpy(header.pmsg,message.c_str(),4);
    header.from = mpi_comm_rank;
    header.to = rank;
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);  
    int idx = 0, length = -1; 
    TYPEARRAY mpi_type = {0x0001,-1,-1,-1};
    int strlen = 0;

    TYPESPACE type_argv;
    // prepare arglist 
    int typechar = *fmt++; 
    char *str;
    int str_len=0;
    while(typechar != 0) {
        if (mpi_type.typecount != 1) mpi_type.typecount = 1; // array item count prefetch, otherwise 1
        mpi_type.typesize = -1; //reset n;
        switch(typechar){
        case 'n': // next arg will be array array lengths next char as array type 
            mpi_type.typecount =  va_arg(args, int);
            mpi_type.typesize =  sizeof(int)*mpi_type.typecount;
            memcpy(buffer + offset, &mpi_type, sizeof(TYPEARRAY));  offset+=sizeof(TYPEARRAY);
            type_argv.int_ptr = (int *)va_arg(args, int *);
            memcpy(buffer + offset, (char *)type_argv.int_ptr, mpi_type.typesize);  offset+=mpi_type.typesize;
            mpi_type.typesize = -1; // reset type size
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
             mpi_type.typesize = sizeof(uint8_t)*mpi_type.typecount;
             MPI_DBG_MSG_PRINTF("MPI_MSG_Sent_CMD()len:%d string %s\n",mpi_type.typecount,type_argv.string_v);// TODO type_argv.string_v);
            //  memcpy(buffer + offset, &mpi_type, sizeof(TYPEARRAY));  offset+=sizeof(TYPEARRAY);
             memcpy((char *)(buffer + offset), type_argv.string_v, mpi_type.typesize);
             offset += sizeof(uint16_t)+mpi_type.typesize;  
             mpi_type.typesize = -1;
            break;   
        default:
            mpi_type.typesize = -1;
            MPI_DBG_ERROR_PRINTF("[MSG]Unsupport format char\n");
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
        typechar=*fmt++;
    }
    va_end(args);
    udp_dump_packet("\n[MPI][MSG]MPI_MSG_Sent_CMD()\n", buffer, offset);
    // MPI_DBG_MSG_PRINTF("MPI_MSG_Sent_CMD(%s)from(%d) to(%d)\n",header.pmsg,header.from,header.to);
    // if (header.to == BROADCAST_RANK || mpi_comm_rank == header.to) {
        // localhost no udp
    //     MPI_MSG_Packet_Dispatch(buffer, offset);
    // }
    // send packet with block mode to remoteIP
    int ret=0;
 //   if (header.to == BROADCAST_RANK || mpi_comm_rank != header.to) {
        int n = udp_Block_Send(buffer,offset);   
        MPI_DBG_MSG_PRINTF("MPI_MSG_Sent_CMD(%s)from(%d) to(%d) ACk%d\n",header.pmsg,header.from,header.to,n);
  //  }
    free(buffer);
    return ret;

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
    header.to = 0; // TODO ?? 
    memcpy(buffer + offset, &header, sizeof(PacketHeader)); 
    offset+=sizeof(PacketHeader);  
  va_list args;
  va_start(args, fmt);
  int length = vsnprintf(buffer+offset, sizebuf, fmt, args); 
  char *pstr = buffer+offset;
  offset+=length; buffer[offset++]=0;
  int n=0;
  va_end(args);  
//   if (mpi_comm_rank==0 ){
//         MPI_MSG_Packet_Dispatch((uint8_t*)buffer, offset); // RUN active at localhost
//         n = udp_Block_Send((uint8_t*)buffer,offset); // extension 

//   }else{
        udp_dump_packet("\n[MPI][MSG]MPI_printf()\n", (uint8_t*)buffer, offset);
        n = udp_Block_Send((uint8_t*)buffer,offset); // extension 
        // MPI_MSG_Sent_CMD("PIN", 0);
//   }
  MPI_DBG_MSG_PRINTF("[MSG]MPI_printf()(%d,%d)len%d %d %s\n",header.from,header.to,offset,n,pstr);
  free(buffer);

}

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
    MPI_DBG_API_PRINTF("[MPI_Init](%d)\n",*argc);
    return MPI_SUCCESS;
}
int MPI_Finalize(void) {
    mpi_initialized = 0;
    MPI_DBG_API_PRINTF("[MPI_Finalize]()\n");
    // if (mpinode) free(mpinode);
    // mpi_comm_size = 0;
    // mpi_comm_rank = BROADCAST_RANK;

    // wait for _udp_arrive_seq == _udp_send_seq

#define UDPCOMPLETETIMEOUT 5000
    unsigned long lastTime = millis();
    unsigned long  currentTime = millis();
    MPI_MSG_Sent_CMD("PIN",255);
    while ((currentTime - lastTime) < UDPCOMPLETETIMEOUT) {
        bool complete_pon = false;
        upd_seq_mutex.lock();
        complete_pon = (_udp_arrive_seq ==  _udp_send_seq);
            MPI_DBG_API_PRINTF("[MPI_Finalize](%d,%d)\n",_udp_send_seq,_udp_arrive_seq);

        upd_seq_mutex.unlock();
        if (complete_pon) break;
        currentTime = millis();
    }
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


/* 
    To provide a comprehensive description of the TX and RX packet life cycle, here’s a detailed breakdown incorporating both the TX control flow and RX flow, along with the main loop handling and semaphore interactions:

    ### TX Packet Life Cycle

    1. **Main Loop (Foreground)**
    - **Create Packet**: Allocate a packet from the pool.
    - **Send Packet**: Send the packet using AsyncUDP and push it into the `outstandingPackets` list.

    2. **RX Task (Polling)**
    - **Check ACK and Timeout**:
        - If the packet is acknowledged, remove it from the list.
        - If the packet times out, retry sending the packet or mark it as failed after a maximum number of retries.
    - **Set Semaphore**: If an ACK is received or the packet times out, set a semaphore to inform the main loop.

    3. **Retry Packet**:
    - In the RX task, if a packet times out, resend the packet.

    4. **Main Loop Handling ACK or Timeout**:
    - Wait for the semaphore.
    - If ACK received, process the ACK data and free the packet.
    - If timeout occurs, handle the timeout scenario and free the packet if necessary.

    5. **Finalize**:
    - Free the packet pool, ensuring that all allocated memory is properly released.

    ### RX Flow

    1. **OnPacket Handler**:
    - **Always Check ACK**: Verify if the incoming packet is an ACK and handle accordingly.
    - **Data Packets**:
        - Validate the data packet.
        - Queue the packet into `rxdata` queue.
        - If the queue is full, mark the RX task as busy until the queue is consumed.
        - Set a semaphore to indicate that RX data is ready for processing.

    2. **RX Task**:
    - **Dispatch Function**: If necessary, process the function related to the received data.
    - **Main Loop Notification**: If the data packet requires main loop handling, set a semaphore to inform the main loop.

    3. **Main Loop Block Receive**:
    - **Check RX Data Queue**: Verify if there is data available in the `rxdata` queue or wait for the semaphore.
    - **Process Requested Data**:
        - Match the ackctx type and source rank.
        - Process the received data.
        - Free the data packet.
    - **Timeout Handling**: If no data is received within the timeout period, handle the failure or continue waiting.

    4. **Finalize RX Data**:
    - Free the data packet to the pool after processing.

    ### Summary

    This comprehensive lifecycle and flow 
    ensure efficient handling of packet transmission and reception, 
    including retries, acknowledgments, and data processing. 
    The use of semaphores and mutexes ensures thread-safe operations 
    and proper synchronization between tasks and the main loop.
*/
#include "MPI_Lite.h"
#include <Freertos/FreeRTOS.h>
#include <Freertos/task.h>
#include <list>
#include <set>
#include <mutex>

// Globals
QueueHandle_t txQueue;
QueueHandle_t rxQueue; // Combined queue for rx and ack
SemaphoreHandle_t txSemaphore;
SemaphoreHandle_t rxSemaphore; // Combined semaphore for rx and ack

std::mutex dispatchMutex;


std::list<MPI_AckContext *> outstandingPackets;
std::mutex outpacketMutex;


std::mutex matchMutex;
std::list<MPI_RecvContext *> recvContexts;
std::list<MPI_Packet *> sentPackets;
SemaphoreHandle_t matchSemaphore;

int max_pong_respones = 0;
std::set<uint32_t> pong_set;


void udp_onPacket(AsyncUDPPacket packet);
void rxTask(void *params);
void rxRetryTask(void *params);
void matchTask(void *params);

// init
bool MPI_udp_init() {
    if (!WiFimDNS_init()) {
        MPI_DBG_ERROR_PRINTF("MPI_udp_init():WiFimDNS_init Fail\n");
        return false;
    }
    if (!MPI_Iot_Setup()){
        MPI_DBG_ERROR_PRINTF("MPI_udp_init():MPI_Iot_Setup Fail\n");
        return false;        
    }

    // Setup UDP Server
    if (udpServer.listen(glistenPort)) {
        MPI_DBG_UDP_PRINTF("MPI_udp_init():Listening on IP: %s, Port: %d\n", WiFi.localIP().toString().c_str(), glistenPort);
        // Register the callback function to handle incoming packets
        udpServer.onPacket(udp_onPacket);
    }

#ifdef __TXTASK__
    txQueue = xQueueCreate(10, sizeof(MPI_Packet *));
    txSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(txTask, "txTask", 2048, NULL, 1, NULL);
#endif
    rxQueue = xQueueCreate(10, sizeof(MPI_Packet *));
    rxSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(rxTask, "rxTask", 2048, NULL, 2, NULL); // higher priority
    xTaskCreate(rxRetryTask, "rxRetryTask", 2048, NULL, 1, NULL); // higher priority

// matching recv and sent
    matchSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(matchTask, "matchTask", 2048, NULL, 1, NULL); // lower priority
    return true;
}


// #### TX Control Flow
// packet is send and 
// status become ONGOING put into outstaning list
// limit to 32 nodes with uint32_t ackmask
bool udp_isMsgPacket(MPI_Packet *packet, const char *msg){
    return (strncmp(packet->pmsg,msg,PACKET_TYPE_SIZE)==0);
}
uint8_t udp_packet_ResetAckBitmasks(MPI_Packet *packet){
    // mapping rank to bitmask
    return 0; // move to SetAckBitmasks
    if (_mpi_state == 0 && (!udp_isMsgPacket(packet,"ACK")) && (!udp_isMsgPacket(packet,"PON")) ) { // unconfig
        packet->to = MPI_BROADCAST_RANK; // unknow ip redirect to broadcast
    }
    // if (_mpi_state == 0) { // unconfig
    //     packet->to = MPI_BROADCAST_RANK;
    // }
    packet->ackmask = (packet->to == MPI_BROADCAST_RANK) ? (uint32_t)-1 : (1<<packet->to);
    if (_mpi_state != 0) { // config
        packet->ackmask = -1;
    } 
    packet->ackmask = (packet->to == MPI_BROADCAST_RANK) ? (uint32_t)-1 : (1<<packet->to);
    return packet->ackmask;
}
uint8_t udp_packetChecksum(MPI_Packet *packet){
    uint8_t checksum = 0;
    uint8_t *pkt = (uint8_t *) packet;
        packet -> checksum = 0;
        for(int i=0; i<packet->length;i++) 
            checksum+=pkt[i];
        checksum = 0 - checksum;
        packet -> checksum = checksum;
        return checksum; // TODO 
}


static uint32_t sequence_number = 1234;
static uint32_t sequence_arrived = 0;

bool udp_isPacketValid(MPI_Packet *packet){
    // checksum check, 
    return true; //TODO mark for test
    uint8_t checksum = 0;
    uint8_t *pkt = (uint8_t *) packet;
    for (int i=0;i<packet->length;i++) {
        checksum += pkt[i];
    }
    if (checksum!=0) return false;
    // if (sequence_arrived > sequence_number) check sequence number if ertry drop it 
    // to bitmask is matched with mpi_comm_rank
    return true;
}

void udp_enQueueRxPacket(MPI_Packet *cpyRxPacket);
MPI_Packet * udp_AllocMpiPacket(MPI_Packet *mpipkt);

int udp_sendPacket(MPI_Packet *packet){
    int n = 0;
    packet->timeStamp = millis();
    packet->seqno = sequence_number++;


    // checksum
    udp_packetChecksum(packet);
    if (packet->to!=MPI_BROADCAST_RANK && packet->remoteip == 0 ){
        MPI_DBG_ERROR_PRINTF("udp_sendPacket() IP error (%s:time%ld:seqno%d:len:%d)  from(%d,%d)\n",packet->pmsg,packet->timeStamp,packet->seqno,packet->length,packet->from,packet->to);
        return 0;
    }

    if (packet->to == mpi_comm_rank){ // localhost only 
        // WAENING  send packet to rxTask will recursive according rxTask retry and packet->to == mpi_comm_rank true

        // ack is done internal in processAckRetry 
        // MPI_Packet *cpyRxPacket = udp_AllocMpiPacket(packet); // allocate and duplicate, packet is ontheway outstanding
        // udp_enQueueRxPacket(cpyRxPacket);
        upd_dispatch_mutex.lock(); // avoid reentry
        MPI_MSG_Packet_Dispatch(packet); 
        upd_dispatch_mutex.unlock();     
        return packet->length;   
        // packet->remoteip=_IPAddressToUInt(IPAddress(127,0,0,1));

    }


    if (packet->to == MPI_BROADCAST_RANK) {
        MPI_DBG_UDP_PRINTF("udp_sendPacket broadcastTo(%s:time%ld:seqno%d:len:%d)  from(%d,%d)\n",packet->pmsg,packet->timeStamp,packet->seqno,packet->length,packet->from,packet->to);

            n = udpClient.broadcastTo((uint8_t *)packet, packet->length, glistenPort); // Use  
    }else{ // if rank is not broadcast, should assign remote ip
        IPAddress ip(packet->remoteip);
        MPI_DBG_UDP_PRINTF("udp_sendPacket writeTo(%s:time%ld:seqno%d:len:%d)  from(%d,%d) ip=%s\n",packet->pmsg,packet->timeStamp,packet->seqno,packet->length,packet->from,packet->to,ip.toString().c_str());
        n = udpClient.writeTo((uint8_t *)packet, packet->length, ip, glistenPort); // Use  
    }
    // local host process later,packet sent first for shorting latency and avoid timeStamp retry
    if (packet->to == MPI_BROADCAST_RANK ) {// localhost loopback for broadcast
        // WARNING  send packet to rxTask will recursive according rxTask retry and packet->to == mpi_comm_rank true
        upd_dispatch_mutex.lock(); // avoid reentry
        MPI_MSG_Packet_Dispatch(packet); 
        upd_dispatch_mutex.unlock();  
        // MPI_Packet *cpyRxPacket = udp_AllocMpiPacket(packet); // allocate and duplicate, broadcast is ontheway
        // udp_enQueueRxPacket(cpyRxPacket); // rxpacket will be free at rxtask

    }

    return n;
}
void udp_logMpiPacket(const char *str,MPI_Packet *mpipkt){
        String msg=String(str)+"("+String(mpipkt->pmsg)+")\n\t";
        MPI_DBG_UDP_PRINTF(msg.c_str());
        MPI_DBG_UDP_PRINTF("time%ld:seqno%d:len:%d)  from(%d,%d) ip(%x) mask(%x)\n",\
         mpipkt->timeStamp,mpipkt->seqno,mpipkt->length,mpipkt->from,mpipkt->to,mpipkt->remoteip,mpipkt->ackmask);
}

MPI_AckContext* udp_addRequestToOutstanding(MPI_Packet *packet) {
    std::lock_guard<std::mutex> lock(outpacketMutex);
    MPI_AckContext *ackctx = (MPI_AckContext *)malloc(sizeof(MPI_AckContext));
    // check bitmask if bit is 
    udp_packet_ResetAckBitmasks(packet);
    ackctx->pkt = packet;
    ackctx->ackSemaphore = xSemaphoreCreateBinary();
    ackctx->pktstate = MSG_STATE_OUTGOING; // change packet state to outgoing
    ackctx->ackstate = MSG_STATE_NULL;  // change packet state to outgoing
    ackctx->lasttime = millis();
    packet->timeStamp = ackctx->lasttime;
    ackctx->trycnt = 0;
    outstandingPackets.push_back(ackctx);
        udp_logMpiPacket("udp_addRequestToOutstanding",packet);
    return ackctx;
}

// call by udp_Block_Sent_Wait_Ack to free Send outstanding packet

int udp_checkAndFreeActContext(MPI_AckContext *ackctx) {
    std::lock_guard<std::mutex> lock(outpacketMutex);
    int state = ackctx->ackstate;
    if (state & (MSG_STATE_TIMEOUT|MSG_STATE_ACK_PON)) {
        MPI_Packet *pkt = ackctx->pkt;
        udp_logMpiPacket("udp_checkAndFreeActContext",pkt);
        outstandingPackets.remove(ackctx);
        // udp_FreeMpiPacket(ackctx->pkt);
        free(ackctx);
    }
    return state;
}

void udp_FreeAllOutRequests() { // finalize
    std::lock_guard<std::mutex> lock(outpacketMutex);
    for (auto& it : outstandingPackets) { // ackRequest

        vSemaphoreDelete(it->ackSemaphore);
        it->ackSemaphore = NULL;
        // free(it->pkt);
        free(it);
    }
    outstandingPackets.clear();
    
}
// duplicate at AsyncUDP onPacket, save to rxQueue
// and free in rxTask or TXD packet in MatchTask 
std::mutex mallocMutex; // for allocate and free

MPI_Packet * udp_AllocMpiPacket(MPI_Packet *mpipkt){
    std::lock_guard<std::mutex> lock(mallocMutex);
    // TODO copy duplicate to free AsyncUDPPacket with memory pool
    // this will be free after processing or Finalize(possible)
    MPI_Packet *mpiPacket=(MPI_Packet *)malloc(mpipkt->length);
    memcpy(mpiPacket, mpipkt, mpipkt->length);
    udp_logMpiPacket("udp_AllocMpiPacket",mpiPacket);
    return mpiPacket;
}
void udp_FreeMpiPacket(MPI_Packet *mpipkt){
    std::lock_guard<std::mutex> lock(mallocMutex);
    udp_logMpiPacket("udp_FreeMpiPacket",mpipkt);
    free(mpipkt);
}

#ifdef __TXTASK__
void txTask(void *params) {
    while (1) {
        if (xSemaphoreTake(txSemaphore, portMAX_DELAY)) {
            MPI_Packet *packet;
            if (xQueueReceive(txQueue, &packet, 0)) {
                // Send packet
                udp_sendPacket(packet); 
                //  outstanding list for further ack, pong, data back checking
                if (packet->answer & MSG_PACKET_REPLY) {// need reply check
                    udp_addRequestToOutstanding(packet);
                }
            }
        }
    }
}
#endif 


// #### RX Flow
bool udp_Reply_AckPonPacket(MPI_Packet *packet, IPAddress remoteIP){
    uint8_t reply = packet->answer & MSG_PACKET_REPLY;
    if (!reply) return false;
    MPI_Packet replyPacket = {"ACK"} ;
    // copy remaind , except pmsg
    memcpy((uint8_t*)(&replyPacket)+PACKET_TYPE_SIZE, \
           (uint8_t*)packet+PACKET_TYPE_SIZE,sizeof(MPI_Packet)-PACKET_TYPE_SIZE);
    bool isPIN = udp_isMsgPacket(packet,"PIN");
    bool isCOM = udp_isMsgPacket(packet,"COM");
    if (isPIN) { // PIN reply PON
            memcpy((uint8_t*)&replyPacket,"PON",PACKET_TYPE_SIZE);
    }
    if (isCOM){
        replyPacket.to = packet->from;   
        replyPacket.from = packet->to; // get owner rank or unconfig reply original root assign
    }else{
        replyPacket.to = packet->from;   
        replyPacket.from = (_mpi_state) ? mpi_comm_rank:packet->to; // get owner rank or unconfig reply original root assign
    }
    replyPacket.answer = 0; // no need reply 
    replyPacket.length = sizeof(MPI_Packet);
    replyPacket.remoteip=_IPAddressToUInt(WifimDNSLocalIP());
    replyPacket.checksum = udp_packetChecksum(&replyPacket); // check sum can caculate offline the use constant define   
    // reply
    udpClient.writeTo((uint8_t *)&replyPacket, replyPacket.length, remoteIP, glistenPort); // Use  
    MPI_DBG_UDP_PRINTF("udp_ReplyIfAckPonPacket(%s) time(%lu),len=%d reply  %s->%s)\n",replyPacket.pmsg,millis(),replyPacket.length,\
              WifimDNSLocalIP().toString().c_str()  ,remoteIP.toString().c_str());
                
    return isPIN;
}
void udp_enQueueRxPacket(MPI_Packet *cpyRxPacket) {
    if (xQueueSend(rxQueue, &cpyRxPacket, portMAX_DELAY) == pdTRUE) {
        MPI_DBG_UDP_PRINTF("udp_onPacket() enQueuu(%s) seqno(%d) from(%s)\n",(char *)cpyRxPacket->pmsg,cpyRxPacket->seqno,cpyRxPacket->remoteip);
        xSemaphoreGive(rxSemaphore);
    }else{
        MPI_DBG_ERROR_PRINTF("udp_onPacket() enQueuu Full, packet loss\n");
        udp_FreeMpiPacket(cpyRxPacket);
    }
}
// AsyncUDPPacket onPacket Handle
void udp_onPacket(AsyncUDPPacket packet) {
    // Check if ACK packet
    MPI_Packet *rxPacket = (MPI_Packet *)packet.data();
    MPI_DBG_UDP_PRINTF("udp_onPacket(%s) time(%lu) seqno(%d) from(%x)\n",(char *)rxPacket->pmsg,millis(),rxPacket->seqno,rxPacket->remoteip);
    if (!udp_isPacketValid(rxPacket)) return; // invalid packet, drop
    if (udp_Reply_AckPonPacket(rxPacket,packet.remoteIP())) return; // reply pon
    MPI_Packet *cpyRxPacket = udp_AllocMpiPacket(rxPacket); // allocate and duplicate
    cpyRxPacket->remoteip=_IPAddressToUInt(packet.remoteIP());
    udp_enQueueRxPacket(cpyRxPacket);
}
bool udp_packet_SetAckBitmasks(MPI_AckContext *ackctx, MPI_Packet *ackpkt){
    MPI_Packet *pkt=ackctx->pkt;
    MPI_DBG_UDP_PRINTF("SetAckBitmasks() seqno(%d) seqno(%d)  ip(%x)\n", pkt->seqno, ackpkt->seqno,ackpkt->remoteip);
    switch(ackctx->ackstate) {
    case MSG_STATE_NULL:
        if (pkt->to == MPI_BROADCAST_RANK) {
            pkt->ackmask = 0;
            if (_mpi_state !=0){ 
                // case 1: config is done, rank list is correct
                for(int rank=0;rank<mpi_comm_size;rank++){
                   pkt->ackmask |= (1<<rank);
                }
                pkt->ackmask &= ~(1<<mpi_comm_rank);
            }else if (pong_set.size()!=0){
                // case 2: with pong_set
                for(int i=0;i<pong_set.size();i++){
                    pkt->ackmask |= (1<<i);
                }

            }else{
            // case 3: wait for timeout
                pkt->ackmask = -1;
            }
        }else {
            pkt->ackmask = (1<<pkt->to);
        }
        ackctx->ackstate = MSG_STATE_ACKING_PONGING; // continue check current ackpacket
        MPI_DBG_UDP_PRINTF("    SetAckBitmasks()  pong_set(%d)  ackip(%x) mask(%d) state(%x)\n",\
                    pong_set.size(), ackpkt->remoteip, ackpkt->ackmask,ackctx->ackstate);
    case MSG_STATE_ACKING_PONGING: // check ack packet 
        MPI_DBG_UDP_PRINTF("    SetAckBitmasks() mask(%d), pkt->from(%d,%d), ackpkt->from(%d,%d) \n", pkt->ackmask,pkt->from,pkt->to,ackpkt->from,ackpkt->to);
        pkt->timeStamp = millis();
        if (pkt->to != MPI_BROADCAST_RANK && pkt->to ==  ackpkt->from) { // one by one, not broadcast
            pkt->ackmask = 0;
            // ackctx->ackstate = MSG_STATE_ACK_PON; // continue check current ackpacket
            MPI_DBG_UDP_PRINTF("    SetAckBitmasks() ackDone mask(%d), pkt->from(%d,%d), ackpkt->from(%d,%d) \n", pkt->ackmask,pkt->from,pkt->to,ackpkt->from,ackpkt->to);

            break;
        }else{ // broadcast
            if (_mpi_state !=0) { // ack from should be single
            // case 1: config is done, rank list is correct
                pkt->ackmask &= ~(1<<ackpkt->from);

            }else if (pong_set.size()!=0){ // try pong feedback
            // case 2: with pong_set
                int i = 0;
                for (const auto& element : pong_set) {
                    if (element == ackpkt->remoteip) pkt->ackmask &= ~(1<<i); 
                    i++;
                    MPI_DBG_UDP_PRINTF("    SetAckBitmasks()  pong_set ip(%x) ackip(%x) ackmask(%x)\n",\
                                element,ackpkt->remoteip, pkt->ackmask);
                }
            }
            // case 3: wait for timeout

        }
        break;
    default:
        break;
    }
    if (pkt->ackmask == 0) {
        MPI_DBG_UDP_PRINTF("  udp_processAck() ackDone %s sno=%d\n",pkt->pmsg,pkt->seqno);
        ackctx->ackstate = MSG_STATE_ACK_PON; // ACK done
        ackctx->lasttime = millis();
        return true; // ack done
    }
    return false;
}

// ACK flow control, update ackctx when ack
void udp_processAck(MPI_Packet *ackPacket){
    std::lock_guard<std::mutex> lock(outpacketMutex);

        MPI_DBG_UDP_PRINTF("udp_processAck() seqno(%d)  ip(%x)\n", ackPacket->seqno,ackPacket->remoteip);
        for (auto& it : outstandingPackets) { // ackRequest
            MPI_Packet *pkt=it->pkt;
            if (it->ackstate & (MSG_STATE_TIMEOUT|MSG_STATE_ACK_PON)) continue;
            if (udp_isMsgPacket(pkt,"PIN")) continue; // process by pong, ack no checking
            if (pkt->seqno == ackPacket->seqno) {
                // Handle packet acknowledgment
                if (udp_packet_SetAckBitmasks(it,ackPacket)){
                    udp_FreeMpiPacket(ackPacket); // free ack packet                
                    xSemaphoreGive(it->ackSemaphore);
                }
                break;
            }
        }
}
void udp_processAckRetry(){
    std::lock_guard<std::mutex> lock(outpacketMutex);
        // check all nonAck and not pin Packets for retry
        // MPI_DBG_UDP_PRINTF("udp_processAckRetry()\n");
        int sizelist = outstandingPackets.size();
        if (sizelist==0) return;
        for (auto& it : outstandingPackets) { // polling all packet for timeout, retry
            MPI_DBG_UDP_PRINTF("udp_processAckRetry() size=%d ackstate(%x)\n",sizelist,it->ackstate);

            if (it->ackstate & (MSG_STATE_TIMEOUT|MSG_STATE_ACK_PON)) continue;
            if (udp_isMsgPacket(it->pkt,"PIN")) {
                continue; // process by pong, ack retry ignore PIN packet
            }
            MPI_Packet *pkt = it->pkt;
            // udp_logMpiPacket("  udp_processAckRetry",pkt);
            if (pkt->from == pkt->to) { // localhost ack
                it->ackstate = MSG_STATE_ACK_PON;
                // localhost no actual ack packet to be free
                xSemaphoreGive(it->ackSemaphore);
            }
            if (millis()-(pkt->timeStamp) > MSG_TIMEOUT_MS) {
                pkt->timeStamp = millis();
                it->trycnt++;
                if (it->trycnt >= MSG_MAX_RETRIES){
                    //set timeout fail 
                    // infor TX timeout FAil 
                    MPI_DBG_UDP_PRINTF("  XXXXXXXudp_processAckRetry timeout(%s)1 (%lu)Timeout seqno(%d) size(%d)  , retry=%d\n",\
                        pkt->pmsg,millis(),pkt->seqno,sizelist,it->trycnt);
                    it->ackstate = MSG_STATE_TIMEOUT;

                    sizelist = outstandingPackets.size();
                    // MPI_DBG_UDP_PRINTF("udp_processAckRetry(%s)2 (%lu)Timeout seqno(%d) size(%d)  , retry=%d\n",\
                    //     it->pmsg,millis(),it->seqno,sizelist,it->trycnt);
                    xSemaphoreGive(it->ackSemaphore);
                    break; // timeout
                }

                MPI_DBG_UDP_PRINTF("  XXXXXXXudp_processAckRetry(%s) retry=%d (%lu)retry seqno(%d)\n",\
                    it->trycnt,\
                    pkt->pmsg,millis(),pkt->seqno);
                it->ackstate = MSG_STATE_RETRY;

                udp_sendPacket(it->pkt); // retry again
            }
        }
}

// PONG flow control
bool udp_checkAckPonState(MPI_AckContext *ackctx, uint32_t state){
    return (ackctx->ackstate == state);
}
// void udp_setAckPonState(MPI_AckContext *ackctx, uint32_t state){
//     ackctx->ackstate = state;
// }
long udp_getAckLastTime(MPI_AckContext *ackctx){
    return ackctx->lasttime;
}

#define MPI_NODE_PON_WAIT_TIME 5000
#define MPI_NODE_RECV_WAIT_TIME 5000

void udp_processPong(MPI_Packet *ponPacket){
        std::lock_guard<std::mutex> lock(outpacketMutex);
        MPI_DBG_UDP_PRINTF("udp_processPong() seqno(%d)  ip(%x)\n", ponPacket->seqno,ponPacket->remoteip);
        for (auto& it : outstandingPackets) { // ackRequest
            MPI_Packet *pkt=it->pkt;
            if (udp_isMsgPacket(pkt,"PIN")){// process by pong, ack no checking
                if (pkt->seqno == ponPacket->seqno) {
                    if (it->ackstate&(MSG_STATE_TIMEOUT|MSG_STATE_ACK_PON)) return; // process done
                    // Handle packet pon state machine , collect pong respone set
                    if (it->ackstate == MSG_STATE_NULL)  {
                        // empty set
                        pong_set.clear();
                    }
                    MPI_DBG_UDP_PRINTF("udp_processPong()  pongsetsize(%x) seqno(%d) ip(%x)\n", pong_set.size(),ponPacket->seqno,ponPacket->remoteip);

                    pong_set.insert(ponPacket->remoteip);
                    pkt->timeStamp = millis();
                    it->ackstate = MSG_STATE_ACKING_PONGING;
                    if (pong_set.size() > max_pong_respones) {
                        max_pong_respones = pong_set.size();
                    }
                    if (pkt->to != MPI_BROADCAST_RANK && pkt->to == ponPacket->from){
                        it->ackstate = MSG_STATE_ACK_PON;
                        xSemaphoreGive(it->ackSemaphore);
                    }
                    break;
                }
            }
        }

}

void udp_processPongWait(){
        std::lock_guard<std::mutex> lock(outpacketMutex);
        if (outstandingPackets.size()==0) return;
        for (auto& it : outstandingPackets) { // ackRequest
            MPI_Packet *pkt=it->pkt;
            if (udp_isMsgPacket(pkt,"PIN")){// process by pong, ack no checking
                if (it->ackstate&(MSG_STATE_TIMEOUT|MSG_STATE_ACK_PON)) return; // process did
                if ((millis()-pkt->timeStamp) > MPI_NODE_PON_WAIT_TIME) { // timeout
                    // Handle packet pon state machine , collect pong respone set
                    MPI_DBG_UDP_PRINTF("udp_processPongWait()f  size(%x) seqno(%d)\n ", pong_set.size(),pkt->seqno);
                    pkt->timeStamp = millis();
                    it->ackstate = MSG_STATE_TIMEOUT;
                    for (const auto& element : pong_set) {
                        MPI_DBG_UDP_PRINTF("udp_processPongWait() pong_set ip(%x)\n",\
                                    element);
                    }
                    xSemaphoreGive(it->ackSemaphore);
                    break;
                }
            }
        }
}

void udp_addSentToList(MPI_Packet *sentpkt);

// Constants for timing
#define TICK_RATE_MS (portTICK_PERIOD_MS)
#define SHORT_WAIT (100 / TICK_RATE_MS)  // 100 milliseconds
// rx dispatch handle, rx packet is free here, except sent rx wait for matching
// 1. ack rx packet
// 2. pon rx packet
// 3. sent rx packet
// 4. others cfg or IO cmd

/*
### Packet Lifecycle and Freeing Criteria

1. **Ack Packet**:
   - **Free at RxTask Ack Checking Done**: After the ack packet has been processed and 
   the acknowledgment has been recorded or acted upon, the packet can be freed.

2. **Pong Packet**:
   - **Free at Pong Waiting Done**: Once the pong packet has fulfilled its purpose 
   (e.g., signaling a response to a ping), and the waiting or processing related to it is completed, 
   the packet can be freed.

3. **Sent Arrival Packet**:
   - **Free after Matched and Buffer Copied by Receive Request Packet**: 
   For data packets that are matched to a receive request, after the data has been copied to the receiving buffer, 
   the packet can be freed.
   - **If Buffer Data Released or Timeout**: 
   If the buffer data is released due to completion or if a timeout occurs, 
   the packet should be freed to prevent memory leaks.

4. **Other IO Pin Control Command Packet**:
   - **Processed by Arrival Dispatch**: For packets that contain control commands, 
   they should be processed upon arrival by dispatching the appropriate IO function.
   - **After Execution of IO Function**: Once the command is executed and 
   no further use of the packet is required, it should be freed.

### Additional Considerations

- **Error Handling**:
  - Ensure that any packets that encounter errors during processing (e.g., invalid packets) 
  are also freed to prevent memory leaks.

- **Retransmissions**:
  - For packets that need to be retransmitted due to a lack of acknowledgment, 
  ensure that retransmission logic handles the freeing of the original packet correctly to avoid memory issues.

- **State Management**:
  - Maintain a clear state for each packet in  lists (e.g., outstanding list, receive list) 
  to track whether they are awaiting acknowledgment, have been acknowledged, 
  or need to be retransmitted.

### Flow Control Summary

1. **OnPacket (AsyncUDP Callback)**:
   - Duplicate the received packet for processing.
   - Based on the packet type, dispatch it to the appropriate handling routine.

2. **Handling Ack Packet**:
   - Process acknowledgment in the RxTask.
   - Free the packet after acknowledgment is processed.

3. **Handling Pong Packet**:
   - Process the pong response.
   - Free the packet after the pong wait is done.

4. **Handling Sent Arrival Packet**:
   - Match with a local receive request.
   - Copy data to the receive buffer.
   - Free the packet after matching and data copying or if a timeout occurs.

5. **Handling IO Control Packet**:
   - Dispatch to the appropriate IO function.
   - Free the packet after executing the command.

By following these detailed steps,  ensure that all packet types are managed efficiently 
and freed appropriately, preventing memory leaks and ensuring proper flow control. 
This detailed approach ensures robustness and efficiency in  MPI-like communication implementation
 on the ESP32 platform.

*/
void rxTask(void *params) {
    while (1) {
        if (xSemaphoreTake(rxSemaphore, SHORT_WAIT)) {
            MPI_Packet *packet;
            if (xQueueReceive(rxQueue, &packet, 0)) {
                MPI_DBG_UDP_PRINTF("rxTask(%s) from(%x)\n",\
                (char *)packet->pmsg,packet->remoteip);
                if (udp_isMsgPacket(packet,"ACK")) {
                    // Process ACK, if ack free ack packet
                    udp_processAck(packet);
                }else if (udp_isMsgPacket(packet,"TXD")) { //TODO  maybe move to dispatch
                    udp_addSentToList(packet); // sentpacket pushback into sentPackets then trigger matchSemaphore
                    // free at matching porcess
                }else if (udp_isMsgPacket(packet,"PON")) { //TODO  maybe move to dispatch
                    udp_processPong(packet);
                    udp_FreeMpiPacket(packet); // free ack packet                
                }else {
                    // Process data packet
                    // Handle RX data packets
                    // all Iot anf CFG and IO functions is processed by Dispatcg
                    // if processed the packet is free, which is allocated in onPacket
                    // the Data arrival check ackctx packet sequence no is matched, inform mainllop by semaphore 
                    upd_dispatch_mutex.lock(); // avoid reentry
                    bool freeflag = MPI_MSG_Packet_Dispatch(packet); 
                    upd_dispatch_mutex.unlock();                
                    // Maybe data Packet is free in main loop or finalize;
                    udp_FreeMpiPacket(packet);
                }
            }
        }
    }
}
void rxRetryTask(void *params) {
    while(1) {
    // process duing size is not zero or time for retry
        // MPI_DBG_UDP_PRINTF("rxTask() try timeout check\n");
        udp_processPongWait();
        udp_processAckRetry();
        vTaskDelay( 500 / portTICK_PERIOD_MS ); 
    }
    
}
/*
 * matching process for recv and sent
 * at functions MPI_RECV and MPI_IRECV
 * 1. create recvContext and push ctx into recvContexts list
 * 2. the Block waiting or test Match semaphore, or timeout
 * 3. get matching Status, then free ctx and recv MPI_packet
 * 
 * at functions MPI_Sent and MPI_ISent
 * 1. push into outstanding List
 * 2. sent packet and create ackContext
 * 3. the block wait or test the ack semaphore, status 
 * 4. remove from outstaning and free the sent packet
 * 
 * matchingTask, interval polling match
 * sentpkt life is ending here
 * recvpkt life is ending at MPI_RECV or MPI_WAIT MPI_TEST
 * 1. if match then 
 *      copy sent data into recv buffer 
 *      free sent MPI_Packet, and remove from recvPacket List
 *      trigger the match semaphore
 * 2. if sentpkt timeout free
 * 
 *  rxTask
 * 1. if one sent packet receive push into SentPackets
 */
void udp_removeSentFromList(MPI_Packet *sentpkt) ;

void udp_processMatchRecvSent() {
    std::lock_guard<std::mutex> lock(matchMutex);
    if (recvContexts.size()==0 || sentPackets.size()==0) return;
    for(auto &rxctx:recvContexts) {
        MPI_Packet *recvpkt = rxctx->pkt;
        for(auto &sentpkt:sentPackets) {
            if ((recvpkt->to==sentpkt->to)&&(recvpkt->comm==sentpkt->comm)&& \
               (recvpkt->tag==sentpkt->tag)&&(recvpkt->count==sentpkt->count)) { //match
                    rxctx->status = MSG_STATE_MATCH;
                    uint8_t* recvBuf ; 
                    memcpy((uint8_t*)&recvBuf,recvpkt->payload,sizeof(uint8_t *)); //get buffer ptr
                    memcpy(recvBuf,sentpkt->payload,sentpkt->length-sizeof(MPI_Packet));
                    udp_removeSentFromList(sentpkt);  
                    xSemaphoreGive(rxctx->matchSemaphore);
            }else if (millis()-recvpkt->timeStamp > MPI_NODE_RECV_WAIT_TIME ){
                // timeout no sent data
                    rxctx->status = MSG_STATE_TIMEOUT;
                    // udp_removeRecvFromList(rxctx->pkt);  move to block wait or async test
                    xSemaphoreGive(rxctx->matchSemaphore);
            }else if (millis()-sentpkt->timeStamp > MPI_NODE_RECV_WAIT_TIME ){
                // timeout no match recv
                    rxctx->status = MSG_STATE_TIMEOUT;
                    udp_removeSentFromList(sentpkt);  
                    xSemaphoreGive(rxctx->matchSemaphore);
            // }
            
            }
        }
    }
}
MPI_RecvContext* udp_addRecvToList(MPI_Packet *packet) {
    std::lock_guard<std::mutex> lock(matchMutex);
    MPI_RecvContext *matchctx = (MPI_RecvContext *)malloc(sizeof(MPI_RecvContext));
    // check bitmask if bit is 
    packet->timeStamp = millis();
    matchctx->pkt = packet;
    matchctx->status = MSG_STATE_WAITING;
    matchctx->matchSemaphore = xSemaphoreCreateBinary();
    recvContexts.push_back(matchctx);
    xSemaphoreGive(matchSemaphore);

    return matchctx;
}
void udp_addSentToList(MPI_Packet *sentpkt) {
    std::lock_guard<std::mutex> lock(matchMutex);
    sentpkt->timeStamp = millis();
    sentPackets.push_back(sentpkt);
    xSemaphoreGive(matchSemaphore);
    return;
}
// after buffer is copy done, for block wait, or for test 
int udp_removeRecvFromList(MPI_RecvContext *matchctx) {
    std::lock_guard<std::mutex> lock(matchMutex);
    int status = matchctx->status;
    if (status & (MSG_STATE_TIMEOUT|MSG_STATE_MATCH)) {
        recvContexts.remove(matchctx);
        // free(matchctx->pkt); // TODO acketbuffer is ready can not be free here
        free(matchctx);
    }
    return status;
}
// free in matchprocess lock already
void udp_removeSentFromList(MPI_Packet *sentpkt) {
        sentPackets.remove(sentpkt);
        udp_FreeMpiPacket(sentpkt);
}
void udp_FreeAllMatchRequests() { // finalize
    std::lock_guard<std::mutex> lock(matchMutex);
    for (auto& it : recvContexts) { // recvContext
        udp_FreeMpiPacket(it->pkt);
        free(it);
    }
    recvContexts.clear();
    for (auto& it : sentPackets) { // send packet 
        free(it);
    }
    sentPackets.clear();
}
void udp_FreeAllRequests() { // finalize
     udp_FreeAllOutRequests();
     udp_FreeAllMatchRequests();
}

// sent rx packet free here
void matchTask(void *params) {
    while(1) {
        if (xSemaphoreTake(matchSemaphore, portMAX_DELAY)){
            udp_processMatchRecvSent();
        }
        vTaskDelay( 100 / portTICK_PERIOD_MS ); 
    }
}




/* TX RX functions*/
int udp_Block_Sent_Wait_Ack(MPI_AckContext *ackctx){
    // wait semaphore
    if (xSemaphoreTake(ackctx->ackSemaphore, portMAX_DELAY)){
        MPI_Packet *packet = ackctx->pkt;
        MPI_DBG_UDP_PRINTF("udp_Block_Sent_Wait_Ack(%s:len:%d)  from(%d,%d) ret(%x)\n",\
                packet->pmsg,packet->length,packet->from,packet->to,ackctx->ackstate);
        int state = udp_checkAndFreeActContext(ackctx);
        return state;
    }
    return MSG_STATE_TIMEOUT;
}
int udp_Block_Recv_Wait_Done(MPI_RecvContext* matchctx){
    // wait semaphore
    if (xSemaphoreTake(matchctx->matchSemaphore, portMAX_DELAY)){
        int state = udp_removeRecvFromList(matchctx);

        // if state is not timeout, the recvpacket buffer has sent data
        return state;
    }
    return MSG_STATE_TIMEOUT;
}
int udp_Block_Send(MPI_Packet *packet){
    //   data Sent    --->              MSG_STATE_OUTGOING
    //                <--- ACK          MSG_STATE_ACK_PON (Done)

    //   data Sent    --->              MSG_STATE_OUTGOING
    //                <-X- ACK          
    //   data Sent    --->              MSG_STATE_RETRY
    //   ...
    //                <-X- ACK          MSG_STATE_TIMEOUT  (Timeout)

        MPI_DBG_UDP_PRINTF("udp_Block_Send(%s:len:%d)  from(%d,%d)\n",packet->pmsg,packet->length,packet->from,packet->to);
        MPI_AckContext* ackctx = udp_addRequestToOutstanding(packet);
        udp_sendPacket(packet);
        int state = udp_Block_Sent_Wait_Ack(ackctx); 
        MPI_DBG_MSG_PRINTF("\tudp_Block_Send done(%s:len:%d)  ackstate(%x) from(%d,%d)\n",packet->pmsg,packet->length,state,packet->from,packet->to);
        return state;
}

int udp_Block_Recv(MPI_Packet* &packet){
      
    //   data request --->              MSG_STATE_RECV_LISTING, push_kack into RECVList
  
    MPI_DBG_UDP_PRINTF("udp_Block_Recv(%s:len:%d)  from(%d,%d)\n",packet->pmsg,packet->length,packet->from,packet->to);
    MPI_RecvContext* recvctx=udp_addRecvToList(packet); // Listen and match
    int state = udp_Block_Recv_Wait_Done(recvctx); 
    MPI_DBG_UDP_PRINTF("udp_Block_Recv(%s:len:%d)  ackstate(%x) from(%d,%d)\n",packet->pmsg,packet->length,state,packet->from,packet->to);
    return state;
}
MPI_Request * udp_Async_Send_Packet(MPI_Packet *packet){
        MPI_AckContext *ackctx = udp_addRequestToOutstanding(packet);
        udp_sendPacket(packet);
        return (MPI_Request *)ackctx;
}

MPI_Request *udp_Async_Recv_Packet(MPI_Packet* packet){
        // recctx is allocated here and free at test
    MPI_RecvContext* recvctx=udp_addRecvToList(packet); // Listen and match
    return (MPI_Request *)recvctx;
}
/*
The `MPI_Test` function can be used for different types of non-blocking operations, 
and its usage depends on whether it is dealing with sending or receiving data.

### Sending Data (Acknowledgment Flow)
When checking a sent data packet, `MPI_Test` is used to monitor the acknowledgment flow. This involves:

1. **Send UDP Packet**: The non-blocking send operation (`MPI_Isend`) sends a packet using UDP.
2. **Outstanding Acknowledgment Check**: The `MPI_Test` function checks the outstanding list 
to see if an acknowledgment (ack) has been received for the sent packet. 
If an ack is received, it signifies that the receiver has successfully received the packet.

### Receiving Data (Listening and Matching Flow)
When checking for data reception, `MPI_Test` is used to match incoming packets with receive requests:

1. **Listening for Incoming Packets**: 
The non-blocking receive operation (`MPI_Irecv`) sets up a request to listen for incoming packets.
2. **Matching Incoming Sent Packets**: 
The `MPI_Test` function checks if an incoming packet matches any of the posted receive requests. 
This involves comparing the source, tag, and communicator to ensure the incoming packet 
is intended for the current receive request.

### Flow Summary

- **Sending (Ack Flow)**:
  - Send data using `MPI_Isend`.
  - Use `MPI_Test` to check if the ack for the sent data has been received.
  - If ack is received, the send operation is complete.

- **Receiving (Listen and Match Flow)**:
  - Post a receive request using `MPI_Irecv`.
  - Use `MPI_Test` to check if an incoming packet matches the posted receive request.
  - If a matching packet is found, the receive operation is complete.


*/
int udp_Test(MPI_Request *request, int *flag, MPI_Status *status){
    MPI_RecvContext* recvctx= (MPI_RecvContext *)request;
    MPI_Packet *packet = recvctx->pkt;
    matchMutex.lock();
    int matchstatus = recvctx->status;
    matchMutex.unlock();
    *flag = 0;
    *status = MPI_FAIL;

    if (packet!=NULL && udp_isMsgPacket(packet,"RXD")) { // receive Request, listen match flow 

        if (matchstatus & (MSG_STATE_TIMEOUT|MSG_STATE_MATCH)) *flag=1;
        if (matchstatus == MSG_STATE_MATCH) *status = MPI_SUCCESS;
        if (matchstatus == MSG_STATE_TIMEOUT) *status = MPI_FAIL;
        if (*flag) {
            udp_FreeMpiPacket(recvctx->pkt); // buffer keep data
            udp_removeRecvFromList((MPI_RecvContext *)recvctx);
            *request = 0;
        }
    } else if (packet!=NULL) { // transfer send Request, ack flow

        if (matchstatus & (MSG_STATE_TIMEOUT|MSG_STATE_ACK_PON)) *flag=1;
        if (matchstatus == MSG_STATE_ACK_PON) *status = MPI_SUCCESS;
        if (matchstatus == MSG_STATE_TIMEOUT) *status = MPI_FAIL;
        if (*flag) {
            udp_checkAndFreeActContext((MPI_AckContext *)recvctx);
            *request = 0;
        }
    }
    return *status;
 
}

/*
  Terry Lee
  完整說明請參閱 http://honeststore.com.tw/esp8266-esp-mesh/
*/
#include "painlessMesh.h"
#include "ESP32inc.h"

#define   MESH_PREFIX     "TinyMesh"
#define   MESH_PASSWORD   "1234"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hi from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  wsTextPrintf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    wsTextPrintf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    wsTextPrintf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    wsTextPrintf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void cmd_mesh_setup() {
    wsTextPrintf("Mesh functions:Setup \n");

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}


int uiState=-1; // start fron setup
int total_node=0;
int selected_node=0;
// esp_mesh_node_list_t nodes;
int cmd_mesh(int argc,char *argv[]){
char buf[64];
char key=0;
int  len=0;
  while(1){
    switch(uiState){
        case -1:// setup
            cmd_mesh_setup();
            uiState = 1;
            break;
        // case 0: // show menu 
        //     wsTextPrintf("Mesh functions:\n");
        //     wsTextPrintf("R:Be Root\n");
        //     wsTextPrintf("L:List Mesh Nodes\n");
        //     for (int i = 0; i < total_node; i++) {
        //         wsTextPrintf("%1d:select node[%d] MAC:",i);
        //         printMacAddress(nodes.node_list[i].mac);
        //     }
        //     wsTextPrintf("?");
        //     uiState = 1;
        //     break;
        case 1: // ask key 
            if (KEYHIT()!=0) { // keyboarinput check
                key=KEYGET();
                if (key==KEY_CRTL_C) return 0;
                if (key=='\x1b') return 0;
                if (key=='q'||key=='Q') return 0;
                KEYGET(); // skip enter
            }else key=0;
            // if ((key-'0')>=0 && (key-'0')<total_node){
            //     select_node = key-'0';
            //     uiState = 1;
            // }
            break;
        // case 2: // ask message
        //     wsTextPrintf("Input message(len<32) to send node[%d] MAC:",i);
        //     wsTextPrintf("?");
        //     uiState = 3;
        //     break;
        // case 3: // enter message
        //     if (KEYGETLINE(buf,&len)) { //done
        //         sendMessageToNode(nodes.node_list[select_node].mac,buf);
        //         uiState = 0; //loop ui again
        //     }
        default:
            break;
        }
        if (uiState>0){
          // it will run the user scheduler as well
            mesh.update();
        }
    }
    return 0;
}

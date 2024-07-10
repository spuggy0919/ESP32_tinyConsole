/*
 * This file is part of ESP32_TinyConsole.
 *
 * ESP32_TinyConsole is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESP32_TinyConsole is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ESP32_TinyConsole.  If not, see 
 *
 *	https://github.com/spuggy0919/ESP32_WEB_CONTROL/blob/espcontrol/LICENSE
 *    GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 *	Author: spuggy0919, spuggy0919@gmail.com
 */
#include "ESP32inc.h"
#include "cmdconfig.h"


#ifdef CMD_MPI
#include <unistd.h>

#include "MPI_Lite.h"

int _force_config(){
    if (_mpi_state == 0){// if not config
        MPI_MSG_Scan_And_Config(); // force config
        return 0;
    } 
    return -1;
}

int cmd_mdns_dump(int argc,char *argv[]){
    _force_config();
    return _MPI_Config_Dump();
}


// MPI_MSG testing "mconfig"
int cmd_mdns_query(int argc,char *argv[]){
    int size,rank;
    if (argc==2) {
      if (String(argv[1])==String("-f")) {
          MPI_MSG_Scan_And_Config(); // force config current node act as ROOT
      }
    }
    MPI_Init(&argc, &argv);
    _MPI_Config_Dump();
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_PRINTF("size=%d,rank=%d\n",size,rank); 
    MPI_Finalize();
    return 0;
}

int cmd_mpi_reboot(int argc,char *argv[]){
     _force_config();
    MPI_Init(&argc, &argv);
    if (argc<2) {
      MPI_PRINTF("Usage mhello rank onoff\n"); 
      MPI_PRINTF("mreboot 255 10000  ; reboot all in 10 sec\n"); 
      MPI_PRINTF("mreboot 2 1000  ; node 2 reboot\n"); 
      return -1;
    }
    int node = atoi(argv[1]);
    int timeout = atoi(argv[2]);
    if (timeout<2000) timeout = 0;
    MPI_MSG_Sent_RST(node,timeout); // reboot caller
    MPI_Finalize();

    return 0;
}
int cmd_mpi_led(int argc,char *argv[]){
     _force_config();
    MPI_Init(&argc, &argv);
    if (argc<2) {
      MPI_PRINTF("Usage mhello rank onoff\n"); 
      MPI_PRINTF("mhello 1 1  ; on\n"); 
      MPI_PRINTF("mhello 1 0  ; off\n"); 
      return -1;
    }
    int node = atoi(argv[1]);
    int onoff = (argc<=2) ? 0:atoi(argv[2]);
    
    MPI_PRINTF("[LED](rank:%d,onoff:%d)\n",node,onoff); 
    if (node>=-1) {
      MPI_MSG_Sent_LED(node,onoff); //turn on led on/off
    }else if (node==-2) { 
      int world_size;
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);

      MPI_MSG_Sent_LED(MPI_BROADCAST_RANK,0); // all off
      delay(1000);
      MPI_MSG_Sent_LED(MPI_BROADCAST_RANK,255); // all off
      delay(1000);
      MPI_MSG_Sent_LED(MPI_BROADCAST_RANK,0); // all off
      delay(1000);
      int pre=0;
      for(int i=0; i<world_size; i++) {
        if (pre!=0) MPI_MSG_Sent_LED(pre,0); // all off
        MPI_MSG_Sent_LED(i,255); // all off
        pre = i;
        delay(1000);
      }
      for(int i=world_size-2; i>=0; i--) {
        if (pre!=0) MPI_MSG_Sent_LED(pre,0); // all off
        MPI_MSG_Sent_LED(i,255); // all off
        pre = i;
        delay(1000);
      }
 
    }
    MPI_Finalize();

    return 0;
}
int parse_options(int argc, char **argv, int *option_f, char **option_n) {
    int i;
    *option_f = 0;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            *option_f = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                *option_n = argv[i + 1];
                i++; // Skip the next argument as it's the value for -b
            } else {
                return -1; // Error: -b option requires an argument
            }
        } else {
            return -1; // Error: Unknown option
        }
    }
    return 0;
}


int cmd_mpi_run(int argc,char *argv[]){  //mpi
    int example = 0;
     _force_config();
    if (argc==2) example = atoi(argv[1]);
    if (example>MPI_TUTORIALS) example = 0;
    return MPI_MSG_Sent_RUN(MPI_BROADCAST_RANK,argv[1]); 
}
#include <cstring>
#include <cctype> // for toupper()

// Function to convert a C-style string to uppercase
void cstr_toUpperCases(char* str) {
    int len = strlen(str);
    
    for (int i = 0; i < len; i++) {
        str[i] = toupper(str[i]); // Convert each character to uppercase
    }
}
int cmd_mpi_msg(int argc,char *argv[]){  //mpi
    //  _force_config();
    int argint[10],n=0;
    int i;
    for( i=2;i<argc;i++){
        argint[n++] =  atoi(argv[i]);
    }
    cstr_toUpperCases(argv[1]);
    MPI_Packet *packet;
    int status=0;
    if ((strncmp(argv[1], "PRT", 4) == 0) || \
         (strncmp(argv[1], "RUN", 4) == 0)) {
        packet = MPI_MSG_Create_Packet(argv[1],argint[0],argv[3]); 
        int status = udp_Block_Send(packet);
        MPI_PRINTF("%s %d %s ret(%d)\n",argv[1],argint[0],argv[3],status); 
        MPI_MSG_Free_Packet(packet);

        return 0;
    }
       if ((strncmp(argv[1], "TXT", 4) == 0) ){
        // TXT rank com tag 
        // int typecode = (int)strtol(argv[6],NULL,16);
        //                              TXT     rank      tag       comm      count
        packet = MPI_MSG_Create_Packet("TXT",argint[0],0,0,16,MPI_CHAR,"hello mpi dump\n"); 
        int status = udp_Block_Send(packet);
        MPI_MSG_Free_Packet(packet);


       }

    switch(n){
    case 0:
      packet = MPI_MSG_Create_Packet(argv[1]); 
      break;
    case 1:
      packet = MPI_MSG_Create_Packet(argv[1],argint[0]); 
      MPI_PRINTF("%s %d \n",argv[1],argint[0]); 
      break;      
    case 2:
      packet = MPI_MSG_Create_Packet(argv[1],argint[0],argint[1]); 
      MPI_PRINTF("%s %d %d\n",argv[1],argint[0],argint[1]); 
      break;      
    case 3:
      packet = MPI_MSG_Create_Packet(argv[1],argint[0],argint[1],argint[2]); 
      break;  
    case 4:
      packet = MPI_MSG_Create_Packet(argv[1],argint[0],argint[1],argint[2],argint[3]); 
      break;  
    default:
      break;
    }
    status = udp_Block_Send(packet);
    MPI_PRINTF("%s %d  ret(%d)\n",argv[1],argint[0],status); 
    MPI_MSG_Free_Packet(packet);

    return 0;
}

// MPI_API testing 
// tutorial 0: MPI Hello World
// mpirun 255 0
int cmd_mpi_hello(int argc,char *argv[]){
    
    MPI_MSG_Sent_DBG(255,0); // turn off debug log

    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_printf("Hello from processor %s, rank %d out of %d processors\n", 
           processor_name, world_rank, world_size);
    MPI_Iot_LED_Blink(world_rank+world_size);


    MPI_Finalize();

    return MPI_SUCCESS;
}



/*
### Example Code mpi_Irxtx_test cmd

In this example:
- Rank 0 sends data to Rank 1 using `MPI_Isend`.
- Rank 1 posts a receive request using `MPI_Irecv`.
- Both ranks use `MPI_Test` to check the completion of their respective operations.

This structure ensures that the sending and receiving operations are correctly 
monitored and completed without blocking the program's execution.
*/
#define BUF_SIZE 100
#define TAG 0

int mpi_Irxtx_test(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        // fprintf(stderr, "This program requires at least two MPI processes.\n");
        MPI_printf( "This program requires at least two MPI processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    MPI_Request send_request, recv_request;
    MPI_Status status;
    char send_buf[BUF_SIZE], recv_buf[BUF_SIZE];
    int flag = 0;

    if (rank == 0) {
        // Initialize send buffer
        snprintf(send_buf, BUF_SIZE, "Hello from rank %d", rank);

        // Non-blocking send to rank 1
        MPI_Isend(send_buf, BUF_SIZE, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, &send_request);

        // Do some computation here
        MPI_printf("Rank %d is doing some computation while waiting for the send to complete.\n", rank);

        // Check if the send is complete
        while (!flag) {
            MPI_Test(&send_request, &flag, &status);
        }

        if (flag) {
            MPI_printf("Rank %d: Send completed.\n", rank);
        }
    } else if (rank == 1) {
        // Non-blocking receive from rank 0
        MPI_Irecv(recv_buf, BUF_SIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &recv_request);

        // Do some computation here
        MPI_printf("Rank %d is doing some computation while waiting for the receive to complete.\n", rank);

        // Check if the receive is complete
        while (!flag) {
            MPI_Test(&recv_request, &flag, &status);
        }

        if (flag) {
            MPI_printf("Rank %d: Received message: %s\n", rank, recv_buf);
        }
    }

    MPI_Finalize();
    return 0;
}
#endif //CMD_MPI
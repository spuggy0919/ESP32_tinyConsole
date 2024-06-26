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
#include "MPI.h"
int _force_config(){
    if (_mpi_state==0){// if not config
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
      MPI_PRINTF("mreboot -1   ; reboot all\n"); 
      MPI_PRINTF("mreboot 2  ; node 2 reboot\n"); 
      return -1;
    }
    MPI_MSG_Sent_RST(atoi(argv[1])); // reboot nodes
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
    int instruction = atoi(argv[1]);
    int onoff = (argc<=2) ? 0:atoi(argv[2]);
    
    MPI_PRINTF("[LED](rank:%d,onoff:%d)\n",instruction,onoff); 
    if (instruction>=-1) {
      MPI_MSG_Sent_LED(instruction,onoff); //turn on led on/off
    }else if (instruction==-2) { 
      int world_size;
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);

      MPI_MSG_Sent_LED(-1,0); // all off
      delay(1000);
      MPI_MSG_Sent_LED(-1,255); // all off
      delay(1000);
      MPI_MSG_Sent_LED(-1,0); // all off
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
int cmd_mpi_exe(int argc,char *argv[]){
    int example = 0;
     _force_config();
    if (argc==2) example = atoi(argv[1]);
    return MPI_MSG_Sent_EXE(example); 
}


// MPI_API testing 
// tutorial 1: MPI Hello World
int cmd_mpi_hello(int argc,char *argv[]){

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

    MPI_Finalize();

    return MPI_SUCCESS;
}
#endif //CMD_MPI
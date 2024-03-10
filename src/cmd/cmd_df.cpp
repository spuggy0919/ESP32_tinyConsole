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
#include <esp_partition.h>
int cmd_df(int argc,char * argv[]){
  char sbuf[256];
  sprintf(sbuf,"ESP32 ChipIModel=%s\nRevision=%x\nChipCores=%d\nClockFreq==%uMZ ",ESP.getChipModel(),ESP.getChipCores(),ESP.getCpuFreqMHz());
  wsTextPrintln(sbuf);
  sprintf(sbuf,"FlashSize=0x%x\nFlashChipSpeed=%u\nFlashChipMode=%x ",ESP.getFlashChipSize(),ESP.getFlashChipSpeed(),ESP.getFlashChipMode());
  wsTextPrintln(sbuf);
  sprintf(sbuf,"PsramSize=%u\nFreePsram=%u\nMinFreePsram=%u\nMaxAllocPsram=%u ",ESP.getPsramSize(),ESP.getFreePsram(),ESP.getMinFreePsram(),ESP.getMaxAllocPsram());
  wsTextPrintln(sbuf);
  wsTextPrintln(reportfs(LittleFS).c_str());
 // Retrieve the partition table
  esp_partition_iterator_t iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  while (iterator) {
    const esp_partition_t *partition = esp_partition_get(iterator);
    
    sprintf(sbuf,"Partition label: %s, Type: %d, Subtype: %d, Size: %x bytes\n",
                  partition->label, partition->type, partition->subtype, partition->size);
    wsTextPrint(sbuf);
    iterator = esp_partition_next(iterator);
  }
    return 0;

}
#ifndef IPC_H
#define IPC_H

#include <Arduino.h>

void ipcInit();

bool ipcPing(uint8_t cpuID);

#endif
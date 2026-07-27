#ifndef CPU_H
#define CPU_H

#include <Arduino.h>

#define MAX_CPUS 4

enum CPUType : uint8_t
{
    CPU_KERNEL = 0,
    CPU_WORKER = 1
};

enum CPUStatus : uint8_t
{
    CPU_OFFLINE = 0,
    CPU_ONLINE = 1
};

struct CPUInfo
{
    uint8_t id;
    uint8_t address;

    CPUType type;
    CPUStatus status;

    uint16_t latency;
};

extern CPUInfo cpuTable[MAX_CPUS];

void cpuInit();
void cpuDetect();

CPUInfo* cpuGet(uint8_t id);

#endif

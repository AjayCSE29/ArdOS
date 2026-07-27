#include "cpu.h"
#include "ipc.h"

CPUInfo cpuTable[MAX_CPUS];

void cpuInit()
{
    for (uint8_t i = 0; i < MAX_CPUS; i++)
    {
        cpuTable[i].id = i;
        cpuTable[i].address = 0;
        cpuTable[i].type = CPU_WORKER;
        cpuTable[i].status = CPU_OFFLINE;
        cpuTable[i].latency = 0;
    }

    // Kernel
    cpuTable[0].type = CPU_KERNEL;
    cpuTable[0].status = CPU_ONLINE;

    // Known workers
    cpuTable[1].address = 0x08;
    cpuTable[2].address = 0x09;
    cpuTable[3].address = 0x0A;
}

CPUInfo* cpuGet(uint8_t id)
{
    if (id >= MAX_CPUS)
        return nullptr;

    return &cpuTable[id];
}

void cpuDetect()
{
    for (uint8_t i = 1; i < MAX_CPUS; i++)
    {
        if (cpuTable[i].address == 0)
            continue;

        if (ipcPing(i))
            cpuTable[i].status = CPU_ONLINE;
        else
            cpuTable[i].status = CPU_OFFLINE;
    }
}


#include <Arduino.h>
#include <Wire.h>

#include "Kernel.h"
#include "shell.h"
#include "ipc.h"
#include "cpu.h"
#include "memory.h"
#include "swap.h"
#include "frame.h"
#include "pager.h"
#include "fs_client.h"

void kernelInit()
{
    Serial.begin(115200);

    while (!Serial);

    Serial.println();
    Serial.println(F("AJAY OS v3"));
    Serial.println();

    Serial.println(F("Booting Kernel..."));
    Serial.println();

    shellInit();
    Serial.println(F("Shell.............OK"));

    ipcInit();
    Serial.println(F("IPC...............OK"));

    cpuInit();
    Serial.println(F("CPU Manager.......OK"));

    cpuDetect();
    Serial.println(F("CPU Detect........OK"));

// ---------------- Filesystem Test ----------------

Serial.println(F("Filesystem........OK"));

fsCreateRemote("hello.txt");
fsCreateRemote("notes.txt");
fsCreateRemote("todo.txt");

FSResponse response = fsListRemote();

Serial.println();
Serial.println(F("Directory Listing"));
Serial.println(F("-----------------"));

if (response.success)
{
    Serial.print(response.data);
}
else
{
    Serial.println(F("Filesystem request failed."));
}
    Serial.println(F(""));
    vmInit();
    Serial.println(F("=== VM TEST ==="));

vmWrite(4, 0, 42);

Serial.print(F("Read: "));
Serial.println(vmRead(4, 0));

vmPrint();

    Serial.println(vmRead(4,0));

    Serial.println();
    Serial.println(F("AjayOS > "));
}

void kernelLoop()
{
    shellUpdate();
}




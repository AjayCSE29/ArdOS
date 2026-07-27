#include "cpu.h"
#include "protocol.h"
#include <Wire.h>

void ipcInit()
{
    Wire.begin();
}

bool ipcPing(uint8_t cpuID)
{
    CPUInfo *cpu = cpuGet(cpuID);

    if (cpu == nullptr)
    {
        Serial.println(F("ERR: NULL CPU"));
        return false;
    }

    Serial.print(F("Address: 0x"));
    Serial.println(cpu->address, HEX);

    Wire.beginTransmission(cpu->address);

    Wire.write((uint8_t)1);
    Wire.write(IPC_PING);

    uint8_t err = Wire.endTransmission();

    Serial.print(F("TX="));
    Serial.println(err);

    if (err != 0)
        return false;

    delay(2);

    Wire.requestFrom(cpu->address, (uint8_t)3);

    Serial.print(F("Bytes="));
    Serial.println(Wire.available());

    if (!Wire.available())
        return false;

    uint8_t status = Wire.read();
    uint8_t length = Wire.read();
    uint8_t reply  = Wire.read();
    
    Serial.print(F("Reply="));
    Serial.println(reply);

    return status == STATUS_OK &&
       length == 1 &&
       reply == DEVICE_WORKER;
}

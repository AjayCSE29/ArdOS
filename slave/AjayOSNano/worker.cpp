#include <Wire.h>
#include "protocol.h"
#include "worker.h"

volatile uint8_t command = 0;
volatile uint8_t parameter = 0;

void receiveEvent(int count)
{
    Serial.print("Bytes received: ");
    Serial.println(count);

    while (Wire.available())
    {
        byte b = Wire.read();

        Serial.print("0x");
        Serial.println(b, HEX);
    }
}

void workerBegin()
{
    Wire.begin(8);
    Wire.onReceive(receiveEvent);
}
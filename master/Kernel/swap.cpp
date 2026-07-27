#include <EEPROM.h>

#include "swap.h"
#include "memory.h"
#include "pager.h"

#include <Arduino.h>

void swapOut(uint8_t page,
             uint8_t frame)
{
    for(uint8_t i = 0; i < PAGE_SIZE; i++)
    {
        EEPROM.update(
            pageTable[page].swapAddress + i,
            physicalMemory[frame][i]
        );
    }

    pageTable[page].present = false;

    swapOuts++;

    Serial.print(F("Swapping OUT Page "));
    Serial.println(page);
}

void swapIn(uint8_t page,
            uint8_t frame)
{
    for(uint8_t i = 0; i < PAGE_SIZE; i++)
    {
        physicalMemory[frame][i] =
            EEPROM.read(
                pageTable[page].swapAddress + i
            );
    }

    pageTable[page].present = true;

    pageTable[page].frame = frame;

    swapIns++;
}

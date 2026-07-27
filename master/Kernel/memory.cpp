#include "memory.h"
#include "swap.h"
#include "pager.h"
#include <EEPROM.h>

PageEntry pageTable[NUM_PAGES];

FrameEntry frameTable[NUM_FRAMES];

uint8_t physicalMemory[NUM_FRAMES][PAGE_SIZE];

uint16_t pageFaults = 0;
uint16_t swapIns = 0;
uint16_t swapOuts = 0;

void vmInit()
{
    for (uint8_t i = 0; i < NUM_PAGES; i++)
    {
        pageTable[i].present = false;
        pageTable[i].dirty = false;
        pageTable[i].referenced = false;

        pageTable[i].frame = 255;

        pageTable[i].swapAddress = i * PAGE_SIZE;
    }

    // Load first four pages into SRAM
    for (uint8_t i = 0; i < NUM_FRAMES; i++)
    {
        pageTable[i].present = true;
        pageTable[i].frame = i;
    }

    for (uint16_t i = 0; i < NUM_PAGES * PAGE_SIZE; i++)
{
    EEPROM.update(i, 0);
}

    for(uint8_t i = 0; i < NUM_FRAMES; i++)
{
    frameTable[i].page = i;
    frameTable[i].occupied = true;

    pageTable[i].present = true;
    pageTable[i].frame = i;
}
}

uint8_t vmRead(uint8_t page,
               uint8_t offset)
{
    if(page >= NUM_PAGES)
        return 0;

    if(offset >= PAGE_SIZE)
        return 0;

    if (!pageTable[page].present)
{
    handlePageFault(page);
}

    pageTable[page].referenced = true;

    return physicalMemory[
        pageTable[page].frame
    ][offset];
}

void vmWrite(uint8_t page,
             uint8_t offset,
             uint8_t value)
{
    if(page >= NUM_PAGES)
        return;

    if(offset >= PAGE_SIZE)
        return;

    if(!pageTable[page].present)
{
    handlePageFault(page);
}

    physicalMemory[
        pageTable[page].frame
    ][offset] = value;

    pageTable[page].dirty = true;
    pageTable[page].referenced = true;
}

void vmPrint()
{
    Serial.println();
    Serial.println(F("========== AjayOS Virtual Memory =========="));
    Serial.println();

    Serial.print(F("Frames        : "));
    Serial.println(NUM_FRAMES);

    Serial.print(F("Virtual Pages : "));
    Serial.println(NUM_PAGES);

    Serial.print(F("Page Size     : "));
    Serial.print(PAGE_SIZE);
    Serial.println(F(" bytes"));

    Serial.print(F("Physical RAM  : "));
    Serial.print(NUM_FRAMES * PAGE_SIZE);
    Serial.println(F(" bytes"));

    Serial.print(F("Virtual Space : "));
    Serial.print(NUM_PAGES * PAGE_SIZE);
    Serial.println(F(" bytes"));

    Serial.println();

    Serial.print(F("Page Faults   : "));
    Serial.println(pageFaults);

    Serial.print(F("Swap Ins      : "));
    Serial.println(swapIns);

    Serial.print(F("Swap Outs     : "));
    Serial.println(swapOuts);

    Serial.println();

    Serial.println(F("----------- Page Table -----------"));

    Serial.println(F("PAGE  FRAME  PRESENT  DIRTY"));

    for(uint8_t i = 0; i < NUM_PAGES; i++)
    {
        Serial.print(F(" "));
        Serial.print(i);
        Serial.print(F("      "));

        if(pageTable[i].present)
            Serial.print(pageTable[i].frame);
        else
            Serial.print(F("-"));

        Serial.print(F("       "));

        if(pageTable[i].present)
            Serial.print(F("YES"));
        else
            Serial.print(F("NO "));

        Serial.print(F("      "));

        if(pageTable[i].dirty)
            Serial.println(F("YES"));
        else
            Serial.println(F("NO"));
    }

    Serial.println();

    Serial.println(F("----------- Frame Table ----------"));

    Serial.println(F("FRAME  PAGE"));

    for(uint8_t i = 0; i < NUM_FRAMES; i++)
    {
        Serial.print(F(" "));
        Serial.print(i);
        Serial.print(F("      "));

        if(frameTable[i].occupied)
            Serial.println(frameTable[i].page);
        else
            Serial.println(F("-"));
    }

    Serial.println();
}

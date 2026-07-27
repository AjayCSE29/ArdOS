#ifndef MEMORY_H
#define MEMORY_H

#include "page.h"
#include "frame.h"

void vmInit();

uint8_t vmRead(uint8_t page, uint8_t offset);

void vmWrite(uint8_t page,
             uint8_t offset,
             uint8_t value);

void vmPrint();

extern PageEntry pageTable[NUM_PAGES];

extern uint8_t physicalMemory[NUM_FRAMES][PAGE_SIZE];

extern uint16_t pageFaults;
extern uint16_t swapIns;
extern uint16_t swapOuts;

extern FrameEntry frameTable[NUM_FRAMES];

#endif

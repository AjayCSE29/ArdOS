#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>

#define PAGE_SIZE      64
#define NUM_FRAMES     4
#define NUM_PAGES      8

struct PageEntry
{
    uint8_t present : 1;
    uint8_t dirty : 1;
    uint8_t referenced : 1;
    uint8_t frame;
    uint16_t swapAddress;
};

#endif

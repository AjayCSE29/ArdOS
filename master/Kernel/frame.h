#ifndef FRAME_H
#define FRAME_H

#include <Arduino.h>

struct FrameEntry
{
    uint8_t page : 7;
    uint8_t occupied : 1;
};

#endif

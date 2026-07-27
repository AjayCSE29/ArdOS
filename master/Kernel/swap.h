#ifndef SWAP_H
#define SWAP_H

#include <Arduino.h>

void swapInit();

void swapOut(uint8_t page, uint8_t frame);
void swapIn(uint8_t page, uint8_t frame);

#endif
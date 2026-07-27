#ifndef PAGER_H
#define PAGER_H

#include <Arduino.h>

uint8_t pagerChooseVictim();

void handlePageFault(uint8_t requestedPage);

#endif
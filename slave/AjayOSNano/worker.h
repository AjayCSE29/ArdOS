#ifndef WORKER_H
#define WORKER_H

#include <Arduino.h>

extern volatile uint8_t command;
extern volatile uint8_t parameter;

void workerBegin();

#endif
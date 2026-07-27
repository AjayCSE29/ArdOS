#include <Wire.h>
#include "Kernel.h"

void setup()
{
    Wire.begin();      // Initialize I²C master

    kernelInit();
}

void loop()
{
    kernelLoop();
}
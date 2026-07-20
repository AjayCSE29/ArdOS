#include "shell.h"

void setup()
{
    Serial.begin(115200);

    while (!Serial);

    bootScreen();
}

void loop()
{
    shell();
}
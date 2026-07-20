#include "shell.h"
#include "filesystem.h"

void setup()
{
    Serial.begin(115200);

    while (!Serial);

    bootScreen();
    fs_init();
}

void loop()
{
    shell();
}
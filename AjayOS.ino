#include "shell.h"
#include "filesystem.h"
#include "ipc.h"
#include "protocol.h"
#include<Wire.h>

void setup()
{
    Serial.begin(115200);
    while(!Serial);

    bootScreen();
    fs_init();
    ipcBegin();

    Serial.println("Distribution working");
}

void loop()
{
    shell();

    if (Serial.available())
    {
        char c = Serial.read();

        if (c == 't')
        {
            Serial.println("Sending Tetris...");
            ipcStartApp(APP_TETRIS);
        }
    }
}
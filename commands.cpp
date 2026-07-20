#include <Arduino.h>
#include "commands.h"

void cmd_help()
{
    Serial.println();
    Serial.println("Available Commands");
    Serial.println("------------------");
    Serial.println("help");
    Serial.println("info");
    Serial.println("clear");
}

void cmd_info()
{
    Serial.println();
    Serial.println("AJAY OS v0.1");
    Serial.println("Processor : ATmega328P");
    Serial.println("Flash     : 32 KB");
    Serial.println("SRAM      : 2 KB");
    Serial.println("EEPROM    : 1 KB");
}

void cmd_clear()
{
    for(int i = 0; i < 40; i++)
    {
        Serial.println();
    }
}
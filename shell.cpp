#include "shell.h"
#include <Arduino.h>
#include "commands.h"

const int MAX_COMMAND_LENGTH = 64;

char inputBuffer[MAX_COMMAND_LENGTH];
int inputLength = 0;

void bootScreen()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("            AJAY OS v0.1");
    Serial.println("========================================");
    Serial.println();

    Serial.println("Processor : ATmega328P");
    Serial.println("Clock     : 16 MHz");
    Serial.println("Flash     : 32 KB");
    Serial.println("SRAM      : 2 KB");
    Serial.println("EEPROM    : 1 KB");

    Serial.println();
    Serial.println("Type 'help' to begin.");
    Serial.println();

    Serial.print("AjayOS > ");
}

void shell()
{
    while (Serial.available())
    {
        char c = Serial.read();

        // ENTER
        if (c == '\r' || c == '\n')
        {
            Serial.println();

            inputBuffer[inputLength] = '\0';

            processCommand(inputBuffer);

            inputLength = 0;

            Serial.print("AjayOS > ");
        }

        // BACKSPACE
        else if (c == 8 || c == 127)
        {
            if (inputLength > 0)
            {
                inputLength--;

                Serial.print("\b \b");
            }
        }

        // NORMAL CHARACTER
        else
        {
            if (inputLength < MAX_COMMAND_LENGTH - 1)
            {
                inputBuffer[inputLength++] = c;

                Serial.print(c);
            }
        }
    }
}

void processCommand(char command[])
{
    if(strcmp(command, "help") == 0)
    {
        cmd_help();
    }
    else if(strcmp(command, "info") == 0)
    {
        cmd_info();
    }
    else if(strcmp(command, "clear") == 0)
    {
        cmd_clear();
    }
    else if(strlen(command) > 0)
    {
        Serial.print("Unknown command: ");
        Serial.println(command);
    }

    Serial.println();
}
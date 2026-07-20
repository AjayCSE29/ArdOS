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
    char* token = strtok(command, " ");

if(token == NULL)
    return;

if(strcmp(token, "help") == 0)
{
    cmd_help();
}
else if(strcmp(token, "info") == 0)
{
    cmd_info();
}
else if(strcmp(token, "clear") == 0)
{
    cmd_clear();
}
else if(strcmp(token, "ls") == 0)
{
    cmd_ls();
}
else if(strcmp(token, "mkdir") == 0)
{
    char* name = strtok(NULL, " ");

    if(name)
        cmd_mkdir(name);
    else
        Serial.println("Usage: mkdir <dirname>");
}
else if(strcmp(token, "touch") == 0)
{
    char* name = strtok(NULL, " ");

    if(name)
        cmd_touch(name);
    else
        Serial.println("Usage: touch <filename>");
}

else if(strcmp(token, "cd") == 0)
{
    char* name = strtok(NULL, " ");

    if(name)
        cmd_cd(name);
    else
        Serial.println("Usage: cd <directory>");
}
else if(strcmp(token, "pwd") == 0)
{
    cmd_pwd();
}

else
{
    Serial.println("Unknown command.");
}

Serial.println();
}
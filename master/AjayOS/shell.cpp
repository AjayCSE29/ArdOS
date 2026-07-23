#include "shell.h"
#include <Arduino.h>
#include "commands.h"
#include "filesystem.h"
#define WRITE_BUFFER_SIZE 128

char writeBuffer[WRITE_BUFFER_SIZE];
int writeLength = 0;
int lineNumber = 1;

ShellState shellState = NORMAL_MODE;

char currentWriteFile[20];

const int MAX_COMMAND_LENGTH = 48;

char inputBuffer[MAX_COMMAND_LENGTH];
int inputLength = 0;

void bootScreen()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("            AJAY OS v2.0.0");
    Serial.println("========================================");
    Serial.println();

    Serial.println("-------Parallel processing enabled-------");
    Serial.println("Processor       : ATmega328P @ 16 MHz");
    Serial.println("Total Clock     : 32 MHz");
    Serial.println("Total Flash     : 64 KB");
    Serial.println("Total SRAM      : 4 KB");
    Serial.println("Total EEPROM    : 2 KB");

    Serial.println();
    Serial.println("Type 'help' to begin.");
    Serial.println();

    Serial.print("AjayOS > ");
}
/*
void shell()
{
    while (Serial.available())
    {
        int c = Serial.read();

        if (c == '\r' || c == '\n')
        {
            Serial.println();
            Serial.print("ENTER");
            Serial.println();
        }
        else
        {
            Serial.write((char)c);
        }
    }
}
*/

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

            if(shellState == NORMAL_MODE)
{
    processCommand(inputBuffer);
}
else if(shellState == WRITE_MODE)
{
    // Save command
    if(strcmp(inputBuffer, ":wq") == 0)
    {
        if(fs_writeFile(currentWriteFile, writeBuffer))
            Serial.println("\nSaved.");
        else
            Serial.println("\nFile not found.");

        shellState = NORMAL_MODE;

        Serial.print("AjayOS > ");
    }
    else
    {
        int len = strlen(inputBuffer);

        // Check if there's enough space
        if(writeLength + len + 2 < WRITE_BUFFER_SIZE)
        {
            strcpy(&writeBuffer[writeLength], inputBuffer);

            writeLength += len;

            writeBuffer[writeLength++] = '\n';

            writeBuffer[writeLength] = '\0';
        }
        else
        {
            Serial.println("\nWrite buffer full.");
        }

        lineNumber++;

        Serial.print(lineNumber);
        Serial.print("> ");
    }
}

            inputLength = 0;
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

else if(strcmp(token, "cat") == 0)
{
    char* name = strtok(NULL, " ");

    if(name)
        cmd_cat(name);
    else
        Serial.println("Usage: cat <filename>");
}

else if(strcmp(token, "write") == 0)
{
    char* name = strtok(NULL, " ");

    if(name)
        cmd_write(name);
    else
        Serial.println("Usage: write <filename>");
}

else if(strcmp(token, "format") == 0)
{
    cmd_format();
}

else if(strcmp(token, "rm") == 0)
{
    char* name = strtok(NULL, " ");

    if(name)
        cmd_rm(name);
    else
        Serial.println("Usage: rm <filename>");
}

else
{
    Serial.println("Unknown command.");
}

Serial.println();
Serial.print("AjayOS > ");
}


void enterWriteMode(const char filename[])
{
    strcpy(currentWriteFile, filename);

    shellState = WRITE_MODE;

    writeLength = 0;
    writeBuffer[0] = '\0';
    lineNumber = 1;

    Serial.println();
    Serial.print("Writing to ");
    Serial.println(filename);
    Serial.println("Type ':wq' to save.");
    Serial.println();

    Serial.print(lineNumber);
    Serial.print("> ");
}
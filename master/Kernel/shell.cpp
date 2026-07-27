#include <Arduino.h>
#include "shell.h"
#include "commands.h"

#define MAX_COMMAND_LENGTH 32

static char commandBuffer[MAX_COMMAND_LENGTH];
static uint8_t commandLength = 0;

void shellInit()
{
    commandLength = 0;
}

void shellUpdate()
{
    while (Serial.available())
    {
        char c = Serial.read();

        // Handle Enter (CR or LF)
        if (c == '\r' || c == '\n')
        {
            // Ignore empty Enter presses
            if (commandLength == 0)
                continue;

            Serial.println();

            // Null-terminate the command string
            commandBuffer[commandLength] = '\0';

            // Execute the command
            executeCommand(commandBuffer);

            // Reset the command buffer
            commandLength = 0;

            // Display the prompt
            Serial.print(F("AjayOS > "));

            continue;
        }

        // Store the character if there's room
        if (commandLength < MAX_COMMAND_LENGTH - 1)
        {
            commandBuffer[commandLength++] = c;
            Serial.print(c); // Echo back to the terminal
        }
    }
}

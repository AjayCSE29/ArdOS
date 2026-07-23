#ifndef SHELL_H
#define SHELL_H

#include <Arduino.h>

enum ShellState
{
    NORMAL_MODE,
    WRITE_MODE
};

void bootScreen();
void shell();
void processCommand(char command[]);
void enterWriteMode(const char filename[]);

#endif
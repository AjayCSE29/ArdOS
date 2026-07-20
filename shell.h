#ifndef SHELL_H
#define SHELL_H

#include <Arduino.h>

void bootScreen();
void shell();
void processCommand(char command[]);

#endif
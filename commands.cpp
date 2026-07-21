#include <Arduino.h>
#include "commands.h"
#include "filesystem.h"
#include "shell.h"

void cmd_help()
{
    Serial.println();
    Serial.println("Available Commands");
    Serial.println("------------------");
    Serial.println("help");
    Serial.println("info");
    Serial.println("clear");
    Serial.println("mkdir");
    Serial.println("touch");
    Serial.println("ls");
    Serial.println("cd");
    Serial.println("pwd");
    Serial.println("format");
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

void cmd_ls()
{
    fs_list();
}

void cmd_mkdir(char name[])
{
    if(fs_createDirectory(name))
        Serial.println("Directory created.");
    else
        Serial.println("Cannot create directory.");
}

void cmd_touch(char name[])
{
    if(fs_createFile(name))
        Serial.println("File created.");
    else
        Serial.println("Cannot create file.");
}

void cmd_cd(char name[])
{
    if(fs_changeDirectory(name))
        return;

    Serial.println("Directory not found.");
}

void cmd_pwd()
{
    fs_printWorkingDirectory();
}

void cmd_write(char filename[])
{
    enterWriteMode(filename);
}

void cmd_cat(char filename[])
{
    if(!fs_readFile(filename))
        Serial.println("File not found.");
}

void cmd_format()
{
    fs_format();
}


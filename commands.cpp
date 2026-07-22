#include <Arduino.h>
#include "commands.h"
#include "filesystem.h"
#include "shell.h"

void cmd_help()
{
    Serial.println();
    Serial.println("Available Commands");
    Serial.println("------------------");
    Serial.println("help        -> View commands");
    Serial.println("info        -> System info");
    Serial.println("clear       -> Clear screen");
    Serial.println("mkdir       -> Create folder");
    Serial.println("touch       -> Create file");
    Serial.println("ls          -> list files and folders");
    Serial.println("cd          -> Change directory");
    Serial.println("pwd         -> Print working directory");
    Serial.println("format      -> Format EEPROM");
    Serial.println("rm          -> Remove file");
    Serial.println("cat         -> Read file");
    Serial.println("write       -> Write file");
    //Serial.println("rm          -> Remove file");

}

void cmd_info()
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

void cmd_rm(char name[])
{
    if(fs_deleteFile(name))
        Serial.println("File deleted.");
    else
        Serial.println("File not found.");
}


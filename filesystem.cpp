#include <Arduino.h>
#include <string.h>

#include "filesystem.h"

const int MAX_ENTRIES = 10;
const int MAX_NAME_LENGTH = 20;

char fileData[512];
uint16_t nextFreeByte = 0;

struct FileEntry
{
    char name[16];

    bool isDirectory;

    int parent;

    uint16_t eepromAddress;
    uint16_t size;
};

FileEntry entries[MAX_ENTRIES];
int entryCount = 0;
int currentDirectory = -1;

void fs_init()
{
    entryCount = 0;
}

bool fs_createDirectory(const char name[])
{
    if(entryCount >= MAX_ENTRIES)
        return false;

    strcpy(entries[entryCount].name, name);
    entries[entryCount].isDirectory = true;
    entries[entryCount].parent = currentDirectory;

    entryCount++;

    return true;
}

bool fs_createFile(const char name[])
{
    if(entryCount >= MAX_ENTRIES)
        return false;

    strcpy(entries[entryCount].name, name);
    entries[entryCount].isDirectory = false;
    entries[entryCount].parent = currentDirectory;
    entries[entryCount].content[0] = '\0';

    entryCount++;

    return true;
}

void fs_list()
{
    Serial.println("Directory: /");
    Serial.println();

    if(entryCount == 0)
    {
        Serial.println("(empty)");
        return;
    }

    for(int i = 0; i < entryCount; i++)
{
    if(entries[i].parent != currentDirectory)
        continue;

    Serial.print(entries[i].name);

    if(entries[i].isDirectory)
        Serial.print("/");

    Serial.println();
}
}

bool fs_changeDirectory(const char name[])
{
    // Go back to root
    if(strcmp(name, "/") == 0)
    {
        currentDirectory = -1;
        return true;
    }

    // Go to parent
    if(strcmp(name, "..") == 0)
    {
        if(currentDirectory != -1)
            currentDirectory = entries[currentDirectory].parent;

        return true;
    }

    // Search for directory
    for(int i = 0; i < entryCount; i++)
    {
        if(entries[i].parent == currentDirectory &&
           entries[i].isDirectory &&
           strcmp(entries[i].name, name) == 0)
        {
            currentDirectory = i;
            return true;
        }
    }

    return false;
}

void fs_printWorkingDirectory()
{
    if(currentDirectory == -1)
    {
        Serial.println("/");
        return;
    }

    Serial.print("/");
    Serial.println(entries[currentDirectory].name);
}

bool fs_writeFile(const char filename[], const char text[])
{
    for(int i = 0; i < entryCount; i++)
    {
        if(entries[i].parent == currentDirectory &&
           !entries[i].isDirectory &&
           strcmp(entries[i].name, filename) == 0)
        {
            strncpy(entries[i].content, text, MAX_FILE_SIZE - 1);

            entries[i].content[MAX_FILE_SIZE - 1] = '\0';

            return true;
        }
    }

    return false;
}

bool fs_readFile(const char filename[])
{
    for(int i = 0; i < entryCount; i++)
    {
        if(entries[i].parent == currentDirectory &&
           !entries[i].isDirectory &&
           strcmp(entries[i].name, filename) == 0)
        {
            for(int j = 0; entries[i].content[j] != '\0'; j++)
            {
                if(entries[i].content[j] == '\n')
                {
                    Serial.println();
                }
                else
                {
                    Serial.print(entries[i].content[j]);
                }
            }

            Serial.println();
            return true;
        }
    }

    return false;
}
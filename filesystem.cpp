#include <Arduino.h>
#include <string.h>

#include "filesystem.h"

const int MAX_ENTRIES = 10;
const int MAX_NAME_LENGTH = 20;

struct FileEntry
{
    char name[20];
    bool isDirectory;

    int parent;
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
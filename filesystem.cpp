#include <Arduino.h>
#include <EEPROM.h>
#include "filesystem.h"

#define DISK_MAGIC      0xA55A
#define HEADER_ADDR     0
#define DIRECTORY_ADDR  16
#define DATA_ADDR       256

const int MAX_ENTRIES = 1;
const int MAX_NAME_LENGTH = 20;

void eepromWriteBlock(int address, const void* data, int size);
void eepromReadBlock(int address, void* data, int size);

void fs_format();
void saveDirectoryEntry(int index);
int getDirectoryAddress(int index);

struct DiskHeader
{
    uint16_t magic;
    uint16_t nextFreeByte;
    uint8_t fileCount;
    uint8_t version;
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
};

struct FileEntry
{
    char name[16];
    bool isDirectory;
    int8_t parent;
    uint16_t start;
    uint16_t size;
};

DiskHeader diskHeader;

FileEntry entries[MAX_ENTRIES];
int entryCount = 0;
int currentDirectory = -1;

int getDirectoryAddress(int index)
{
    return DIRECTORY_ADDR + (index * sizeof(FileEntry));
}

void saveDirectoryEntry(int index)
{
    eepromWriteBlock(
        getDirectoryAddress(index),
        &entries[index],
        sizeof(FileEntry)
    );
}

void eepromWriteBlock(int address, const void* data, int size)
{
    const byte* ptr = (const byte*)data;

    for(int i = 0; i < size; i++)
    {
        EEPROM.update(address + i, ptr[i]);
    }
}

void eepromReadBlock(int address, void* data, int size)
{
    byte* ptr = (byte*)data;

    for(int i = 0; i < size; i++)
    {
        ptr[i] = EEPROM.read(address + i);
    }
}

void fs_format()
{
    // Clear the entire EEPROM
    for(int i = 0; i < EEPROM.length(); i++)
    {
        EEPROM.update(i, 0);
    }

    // Initialize the disk header
    diskHeader.magic = DISK_MAGIC;
    diskHeader.nextFreeByte = DATA_ADDR;
    diskHeader.fileCount = 0;
    diskHeader.version = 1;

    eepromWriteBlock(
        HEADER_ADDR,
        &diskHeader,
        sizeof(DiskHeader)
    );

    entryCount = 0;
    currentDirectory = -1;

    Serial.println("Filesystem formatted.");
}

void fs_init()
{
    eepromReadBlock(HEADER_ADDR, &diskHeader, sizeof(DiskHeader));

    if(diskHeader.magic != DISK_MAGIC)
    {
        Serial.println("Formatting EEPROM...");

        fs_format();

        Serial.println("Done.");
    }
    else
    {
        Serial.println("Filesystem OK.");
    }   

    // Load directory entries from EEPROM
    entryCount = diskHeader.fileCount;

    for(int i = 0; i < entryCount; i++)
    {
        eepromReadBlock(
            getDirectoryAddress(i),
            &entries[i],
            sizeof(FileEntry)
        );
    }
}

bool fs_createDirectory(const char name[])
{
    if(entryCount >= MAX_ENTRIES)
        return false;

    strcpy(entries[entryCount].name, name);
    entries[entryCount].isDirectory = true;
    entries[entryCount].parent = currentDirectory;
    entries[entryCount].start = 0;
    entries[entryCount].size = 0;

    saveDirectoryEntry(entryCount);

    diskHeader.fileCount++;

    eepromWriteBlock(
        HEADER_ADDR,
        &diskHeader,
        sizeof(DiskHeader)
    );

    entryCount++;

    return true;
}

bool fs_createFile(const char name[])
{
    if(entryCount >= MAX_ENTRIES)
        return false;

    // Check if the file already exists
    for(int i = 0; i < entryCount; i++)
    {
        if(strcmp(entries[i].name, name) == 0 &&
           entries[i].parent == currentDirectory)
        {
            return false;
        }
    }

    strcpy(entries[entryCount].name, name);
    entries[entryCount].isDirectory = false;
    entries[entryCount].parent = currentDirectory;
    entries[entryCount].start = 0;
    entries[entryCount].size = 0;

    saveDirectoryEntry(entryCount);

    diskHeader.fileCount++;

    eepromWriteBlock(
        HEADER_ADDR,
        &diskHeader,
        sizeof(DiskHeader)
    );

Serial.print("Saving file count: ");
Serial.println(diskHeader.fileCount);

eepromWriteBlock(
    HEADER_ADDR,
    &diskHeader,
    sizeof(DiskHeader)
);

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

bool fs_writeFile(const char* filename, const char* text)
{
    for(int i = 0; i < entryCount; i++)
    {
        if(strcmp(entries[i].name, filename) == 0)
        {
            if(entries[i].start == 0)
            {
                entries[i].start = diskHeader.nextFreeByte;
            }

            int len = strlen(text);

            for(int j = 0; j < len; j++)
            {
                EEPROM.update(entries[i].start + j, text[j]);
            }

            entries[i].size = len;

            diskHeader.nextFreeByte = entries[i].start + len;

            saveDirectoryEntry(i);

            eepromWriteBlock(HEADER_ADDR, &diskHeader, sizeof(DiskHeader));

            return true;
        }
    }

    return false;
}

bool fs_readFile(const char* filename)
{
    for(int i = 0; i < entryCount; i++)
    {
        if(strcmp(entries[i].name, filename) == 0)
        {
            for(int j = 0; j < entries[i].size; j++)
            {
                char c = EEPROM.read(entries[i].start + j);

                if(c == '\n')
                    Serial.println();
                else
                    Serial.print(c);
            }

            Serial.println();

            return true;
        }
    }

    return false;
}

bool fs_deleteFile(const char name[])
{
    int index = -1;

    // Find the file
    for(int i = 0; i < entryCount; i++)
    {
        if(!entries[i].isDirectory &&
           entries[i].parent == currentDirectory &&
           strcmp(entries[i].name, name) == 0)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
        return false;

    // Shift remaining entries
    for(int i = index; i < entryCount - 1; i++)
    {
        entries[i] = entries[i + 1];

        saveDirectoryEntry(i);
    }

    entryCount--;
    diskHeader.fileCount = entryCount;

    // Clear the last directory entry in EEPROM
    FileEntry empty = {};
    eepromWriteBlock(
        getDirectoryAddress(entryCount),
        &empty,
        sizeof(FileEntry)
    );

    // Save updated header
    eepromWriteBlock(
        HEADER_ADDR,
        &diskHeader,
        sizeof(DiskHeader)
    );

    return true;
}
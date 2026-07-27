#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>

#define MAX_FILES       16
#define MAX_FILENAME    16
#define MAX_FILE_SIZE   64

struct FileEntry
{
    bool used;
    char name[MAX_FILENAME];
    uint8_t size;
    uint8_t data[MAX_FILE_SIZE];
};

void fsInit();

bool fsCreate(const char *name);
bool fsDelete(const char *name);
bool fsExists(const char *name);

bool fsWrite(const char *name,const char *text);
bool fsRead(const char *name,char *buffer);

void fsBuildDirectoryListing(char *buffer,uint8_t maxLength);

#endif
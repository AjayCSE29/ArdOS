#ifndef FS_CLIENT_H
#define FS_CLIENT_H

#include <Arduino.h>

struct FSResponse
{
    bool success;
    uint8_t status;
    char data[32];
};

FSResponse fsRequest(uint8_t opcode,const char *payload);

FSResponse fsCreateRemote(const char *filename);

FSResponse fsDeleteRemote(const char *filename);

FSResponse fsReadRemote(const char *filename);

FSResponse fsWriteRemote(const char *filename, const char *text);

FSResponse fsExistsRemote(const char *filename);

FSResponse fsListRemote();

#endif

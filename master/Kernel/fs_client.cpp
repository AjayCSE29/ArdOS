#include <Wire.h>

#include "fs_client.h"
#include "protocol.h"

#define FS_ADDRESS 0x08

FSResponse fsRequest(uint8_t opcode, const char *payload)
{
    FSResponse r;

    r.success = false;
    r.status = STATUS_ERROR;
    r.data[0] = '\0';

    Wire.beginTransmission(FS_ADDRESS);

    Wire.write(opcode);

    if (payload != nullptr)
    {
        while (*payload)
            Wire.write(*payload++);
    }

    Wire.endTransmission();

    delay(2);

    Wire.requestFrom(FS_ADDRESS, (uint8_t)32);

    if (Wire.available() < 2)
        return r;

    r.status = Wire.read();
    r.success = (r.status == STATUS_OK);

    uint8_t len = Wire.read();

    uint8_t i = 0;

    while (Wire.available() && i < len && i < sizeof(r.data) - 1)
    {
        r.data[i++] = Wire.read();
    }

    r.data[i] = '\0';

    return r;
}

FSResponse fsCreateRemote(const char *filename)
{
    return fsRequest(FS_CREATE, filename);
}

FSResponse fsDeleteRemote(const char *filename)
{
    return fsRequest(FS_DELETE, filename);
}

FSResponse fsReadRemote(const char *filename)
{
    return fsRequest(FS_READ, filename);
}

FSResponse fsWriteRemote(const char *filename, const char *text)
{
    char payload[48];
    uint8_t i = 0;

    while (*filename && i < sizeof(payload) - 2)
        payload[i++] = *filename++;

    payload[i++] = ' ';

    while (*text && i < sizeof(payload) - 1)
        payload[i++] = *text++;

    payload[i] = '\0';

    return fsRequest(FS_WRITE, payload);
}

FSResponse fsExistsRemote(const char *filename)
{
    return fsRequest(FS_EXISTS, filename);
}

FSResponse fsListRemote()
{
    FSResponse r = fsRequest(FS_LIST, "");
    if (!r.success)
        r = fsRequest(FS_LIST, "");
    return r;
}

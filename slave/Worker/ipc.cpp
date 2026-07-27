#include <Wire.h>
#include <string.h>

#include "ipc.h"
#include "filesystem.h"
#include "protocol.h"

#define FS_ADDRESS 0x08
#define RESPONSE_BUFFER_SIZE 32

static uint8_t responseBuffer[RESPONSE_BUFFER_SIZE];
static uint8_t responseLength = 0;

void receiveEvent(int bytes);
void requestEvent();

static void makeResponse(uint8_t status,const char *text)
{
    responseBuffer[0] = status;

    uint8_t len = 0;

    if(text != nullptr)
    {
        len = strlen(text);

        if(len > RESPONSE_BUFFER_SIZE - 2)
            len = RESPONSE_BUFFER_SIZE - 2;

        memcpy(&responseBuffer[2], text, len);
    }

    responseBuffer[1] = len;
    responseLength = len + 2;
}

void ipcInit()
{
    Wire.begin(FS_ADDRESS);

    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void receiveEvent(int bytes)
{
    if(bytes <= 0)
        return;

    uint8_t opcode = Wire.read();

    switch(opcode)
    {
        case 0x10:
        {
            char filename[MAX_FILENAME];

            int i = 0;

            while(Wire.available() && i < MAX_FILENAME-1)
                filename[i++] = Wire.read();

            filename[i]='\0';

            if (fsExists(filename))
                makeResponse(ERR_ALREADY_EXISTS, nullptr);
            else if (fsCreate(filename))
                makeResponse(STATUS_OK, nullptr);
            else
                makeResponse(ERR_DISK_FULL, nullptr);

            break;
        }

        case 0x11:
        {
            char filename[MAX_FILENAME];

            int i = 0;

            while(Wire.available() && i < MAX_FILENAME-1)
                filename[i++] = Wire.read();

            filename[i]='\0';

            if (fsDelete(filename))
                makeResponse(STATUS_OK, nullptr);
            else
                makeResponse(ERR_NOT_FOUND, nullptr);

            break;
        }

        case 0x12:
        {
            char list[120];

            fsBuildDirectoryListing(list,sizeof(list));

            makeResponse(STATUS_OK, list);

            break;
        }

        case 0x13:
        {
            char filename[MAX_FILENAME];

            int i = 0;

            while(Wire.available() && i < MAX_FILENAME-1)
                filename[i++] = Wire.read();

            filename[i]='\0';

            char fileData[MAX_FILE_SIZE];

            if (fsRead(filename, fileData))
                makeResponse(STATUS_OK, fileData);
            else
                makeResponse(ERR_NOT_FOUND, nullptr);

            break;
        }

        case 0x14:
        {
            char payload[48];

            int i = 0;

            while(Wire.available() && i < sizeof(payload)-1)
                payload[i++] = Wire.read();

            payload[i]='\0';

            char *space = strchr(payload, ' ');

            if (space)
            {
                *space = '\0';
                if (fsWrite(payload, space + 1))
                    makeResponse(STATUS_OK, nullptr);
                else
                    makeResponse(ERR_NOT_FOUND, nullptr);
            }
            else
            {
                makeResponse(ERR_INVALID_NAME, nullptr);
            }

            break;
        }

        case 0x15:
        {
            char filename[MAX_FILENAME];

            int i = 0;

            while(Wire.available() && i < MAX_FILENAME-1)
                filename[i++] = Wire.read();

            filename[i]='\0';

            if (fsExists(filename))
                makeResponse(STATUS_OK, nullptr);
            else
                makeResponse(ERR_NOT_FOUND, nullptr);

            break;
        }
        
        case IPC_PING:
{
    responseBuffer[0] = STATUS_OK;
    responseBuffer[1] = 1;               // One byte of data
    responseBuffer[2] = DEVICE_WORKER;   // Worker identifier
    responseLength = 3;
    break;
}

        default:
        {
            makeResponse(STATUS_ERROR, "Unknown Opcode");
            break;
        }
    }
}

void requestEvent()
{
    Wire.write(responseBuffer, responseLength);
}
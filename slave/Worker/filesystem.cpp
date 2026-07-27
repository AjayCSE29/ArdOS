#include "filesystem.h"

static FileEntry files[MAX_FILES];

void fsInit()
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        files[i].used = false;
        files[i].size = 0;
        files[i].name[0] = '\0';
    }
}

bool fsExists(const char* name)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].used && strcmp(files[i].name, name) == 0)
        {
            return true;
        }
    }

    return false;
}

bool fsCreate(const char* name)
{
    if (fsExists(name))
        return false;

    for (int i = 0; i < MAX_FILES; i++)
    {
        if (!files[i].used)
        {
            files[i].used = true;

            strncpy(files[i].name, name, MAX_FILENAME - 1);
            files[i].name[MAX_FILENAME - 1] = '\0';

            files[i].size = 0;

            memset(files[i].data, 0, MAX_FILE_SIZE);

            return true;
        }
    }

    return false;
}

bool fsDelete(const char* name)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].used && strcmp(files[i].name, name) == 0)
        {
            files[i].used = false;
            files[i].size = 0;
            files[i].name[0] = '\0';
            memset(files[i].data, 0, MAX_FILE_SIZE);

            return true;
        }
    }

    return false;
}

bool fsWrite(const char* name, const char* text)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].used && strcmp(files[i].name, name) == 0)
        {
            size_t len = strlen(text);

            if (len >= MAX_FILE_SIZE)
                len = MAX_FILE_SIZE - 1;

            memcpy(files[i].data, text, len);
            files[i].data[len] = '\0';
            files[i].size = len;

            return true;
        }
    }

    return false;
}

bool fsRead(const char* name, char* buffer)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].used && strcmp(files[i].name, name) == 0)
        {
            strcpy(buffer, (char*)files[i].data);
            return true;
        }
    }

    return false;
}

void fsList()
{
    Serial.println("Files:");
    Serial.println("----------------");

    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].used)
        {
            Serial.print(files[i].name);
            Serial.print(" (");
            Serial.print(files[i].size);
            Serial.println(" bytes)");
        }
    }

    Serial.println("----------------");
}

void fsBuildDirectoryListing(char *buffer, uint8_t maxLength)
{
    buffer[0] = '\0';

    for (int i = 0; i < MAX_FILES; i++)
    {
        if (!files[i].used)
            continue;

        if (strlen(buffer) + strlen(files[i].name) + 3 >= maxLength)
            break;

        strcat(buffer, files[i].name);
        strcat(buffer, "\r\n");
    }
}
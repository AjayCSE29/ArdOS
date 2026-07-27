#include <Arduino.h>
#include <string.h>

#include "cpu.h"
#include "commands.h"
#include "ipc.h"
#include "memory.h"
#include "fs_client.h"
#include "protocol.h"

void bootScreen()
{
    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("            AJAY OS v2.0.0"));
    Serial.println(F("========================================"));
    Serial.println(F(" "));

    Serial.println(F("-------Parallel processing enabled-------"));
    Serial.println(F("Processor       : ATmega328P @ 16 MHz"));
    Serial.println(F("Total Clock     : 32 MHz"));
    Serial.println(F("Total Flash     : 64 KB"));
    Serial.println(F("Total SRAM      : 4 KB"));
    Serial.println(F("Total EEPROM    : 2 KB"));

    Serial.println(F(" "));
    Serial.println(F("Type 'help' to begin."));
    Serial.println(F(" "));

    Serial.print(F("AjayOS > "));
}

void cmd_vm()
{
    vmPrint();
}

void executeCommand(char *command)
{
    // Split command into tokens
    char *cmd  = strtok(command, " ");
    char *arg1 = strtok(NULL, " ");
    char *arg2 = strtok(NULL, "");

    if (cmd == nullptr)
        return;
    
    if(strcmp(cmd, "info") == 0){
        bootScreen();
    }

    if (strcmp(cmd, "help") == 0)
    {
        Serial.println(F("Available Commands"));
        Serial.println(F("------------------"));
        Serial.println(F("help"));
        Serial.println(F("ping"));
        Serial.println(F("cpu"));
        Serial.println(F("echo"));
        Serial.println(F("clear"));
        Serial.println(F("vm"));
        Serial.println(F("touch"));
        Serial.println(F("ls"));
        Serial.println(F("cat"));
        Serial.println(F("write"));
        Serial.println(F("rm"));
        Serial.println(F("exists"));
        Serial.println(F("------------------"));
    }

    else if (strcmp(cmd, "ping") == 0)
    {
        Serial.println(F("Pinging CPU1..."));

        unsigned long start = millis();

        if (ipcPing(1))
        {
            Serial.print(F("Reply received in "));
            Serial.print(millis() - start);
            Serial.println(F(" ms"));
        }

        if (ipcPing(2))
        {
            Serial.print(F("Reply received in "));
            Serial.print(millis() - start);
            Serial.println(F(" ms"));
        }
        else
        {
            Serial.println(F("No response."));
        }
    }

    else if (strcmp(cmd, "echo") == 0)
    {
        if (arg1)
            Serial.println(arg1);
    }

    else if (strcmp(cmd, "clear") == 0)
    {
        for (int i = 0; i < 50; i++)
            Serial.println();
    }

    else if (strcmp(cmd, "cpu") == 0)
    {
        Serial.println();
        Serial.println(F("ID  TYPE     STATUS"));

        for (uint8_t i = 0; i < MAX_CPUS; i++)
        {
            CPUInfo *cpu = cpuGet(i);

            Serial.print(cpu->id);
            Serial.print(F("   "));

            if (cpu->type == CPU_KERNEL)
                Serial.print(F("KERNEL   "));
            else
                Serial.print(F("WORKER   "));

            if (cpu->status == CPU_ONLINE)
                Serial.println(F("ONLINE"));
            else
                Serial.println(F("OFFLINE"));
        }
    }

    else if (strcmp(cmd, "vm") == 0)
    {
        cmd_vm();
    }

    else if (strcmp(cmd, "touch") == 0)
    {
        if (!arg1)
        {
            Serial.println(F("Usage: touch <filename>"));
        }
        else
        {
            FSResponse response = fsCreateRemote(arg1);

            if (response.status == STATUS_OK)
                Serial.println(F("File created."));
            else if (response.status == ERR_ALREADY_EXISTS)
                Serial.println(F("File already exists."));
            else if (response.status == ERR_DISK_FULL)
                Serial.println(F("Disk full."));
            else
                Serial.println(F("Failed to create file."));
        }
    }

    else if (strcmp(cmd, "ls") == 0)
    {
        FSResponse response = fsListRemote();

        if (response.status == STATUS_OK)
        {
            Serial.println(response.data);
        }
        else
        {
            Serial.println(F("Filesystem request failed."));
        }
    }

    else if (strcmp(cmd, "cat") == 0)
    {
        if (arg1)
        {
            FSResponse response = fsReadRemote(arg1);

            if (response.status == STATUS_OK)
                Serial.println(response.data);
            else if (response.status == ERR_NOT_FOUND)
                Serial.println(F("File not found."));
            else
                Serial.println(F("Read failed."));
        }
        else
        {
            Serial.println(F("Usage: cat <filename>"));
        }
    }

    else if (strcmp(cmd, "write") == 0)
    {
        if (arg1 && arg2)
        {
            FSResponse response = fsWriteRemote(arg1, arg2);

            if (response.status == STATUS_OK)
                Serial.println(F("Written."));
            else if (response.status == ERR_NOT_FOUND)
                Serial.println(F("File not found."));
            else if (response.status == ERR_INVALID_NAME)
                Serial.println(F("Invalid filename."));
            else
                Serial.println(F("Write failed."));
        }
        else
        {
            Serial.println(F("Usage: write <filename> <text>"));
        }
    }

    else if (strcmp(cmd, "rm") == 0)
    {
        if (arg1)
        {
            FSResponse response = fsDeleteRemote(arg1);

            if (response.status == STATUS_OK)
                Serial.println(F("Removed."));
            else if (response.status == ERR_NOT_FOUND)
                Serial.println(F("File not found."));
            else
                Serial.println(F("Delete failed."));
        }
        else
        {
            Serial.println(F("Usage: rm <filename>"));
        }
    }

    else if (strcmp(cmd, "exists") == 0)
    {
        if (arg1)
        {
            FSResponse response = fsExistsRemote(arg1);

            if (response.status == STATUS_OK)
                Serial.println(F("Exists"));
            else if (response.status == ERR_NOT_FOUND)
                Serial.println(F("Missing"));
            else
                Serial.println(F("Check failed."));
        }
        else
        {
            Serial.println(F("Usage: exists <filename>"));
        }
    }

    else
    {
        Serial.println(F("Unknown command."));
    }
}

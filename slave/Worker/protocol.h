#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define WORKER_ADDRESS 0x08

enum Command
{
    CMD_NONE = 0,

    CMD_PING = 1,
    CMD_PONG = 2,

    CMD_STATUS = 3,

    CMD_ERROR = 255
};

struct Packet
{
    uint8_t command;
    uint8_t source;
    uint8_t destination;
    uint8_t length;
    char data[28];
};

#define FS_CREATE      0x10
#define FS_DELETE      0x11
#define FS_LIST        0x12
#define FS_READ        0x13
#define FS_WRITE       0x14
#define FS_EXISTS      0x15

// Generic Status Codes
#define STATUS_OK      0x00
#define STATUS_ERROR   0x01
#define ERR_NOT_FOUND      0x02
#define ERR_ALREADY_EXISTS 0x03
#define ERR_DISK_FULL      0x04
#define ERR_INVALID_NAME   0x05

// System Commands
#define IPC_PING      0x01
#define SYS_IDENTIFY  0x02

// Device Types
#define DEVICE_KERNEL      0x01
#define DEVICE_WORKER      0x02
#define DEVICE_FILESYSTEM  0x03
#define DEVICE_DISPLAY     0x04
#define DEVICE_UNKNOWN     0xFF

#endif
#ifndef PROTOCOL_H
#define PROTOCOL_H

// Kernel -> Worker
#define CMD_START_APP   0x01
#define CMD_KEYPRESS    0x02
#define CMD_STOP_APP    0x03
#define CMD_PING        0x04
#define CMD_STATUS      0x05

// Worker -> Kernel
#define RESP_FRAME      0x80
#define RESP_SCORE      0x81
#define RESP_EXIT       0x82
#define RESP_PONG       0x83
#define RESP_STATUS     0x84

// Applications
#define APP_TETRIS      0x01

#endif
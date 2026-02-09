#pragma once

#define MAX_TEXT 1024
#define MAX_NAME 32

typedef enum {
    MSG_HELLO,
    MSG_CHAT,
    MSG_COMMAND,
    MSG_SYSTEM
} MessageType;

typedef struct {
    MessageType type;
    char sender[MAX_NAME];
    char text[MAX_TEXT];
} Message;

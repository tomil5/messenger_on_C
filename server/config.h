#pragma once
#include "../common/message.h"

#define PORT 5000
#define MAX_CLIENTS 10

typedef struct {
    int sock;
    char name[MAX_NAME];
} Client;

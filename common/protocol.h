#pragma once
#include "message.h"

int send_message(int sock, Message *msg);
int recv_message(int sock, Message *msg);

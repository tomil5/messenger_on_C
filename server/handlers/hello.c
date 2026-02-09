#include "../../common/message.h"
#include "../config.h"
#include "../dispatcher.h"
#include <string.h>
#include <stdio.h>

int send_message(int sock, Message *msg);

void handle_hello(Client clients[], Client *c, Message *msg) {
    strncpy(c->name, msg->text, MAX_NAME);
    Message out;
    out.type = MSG_SYSTEM;
    strncpy(out.sender, "Server", MAX_NAME);
    snprintf(out.text, MAX_TEXT, "%s joined the chat!", c->name);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sock) send_message(clients[i].sock, &out);
    }
}

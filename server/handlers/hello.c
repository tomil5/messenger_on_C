#include "../../common/message.h"
#include <string.h>

typedef struct {
    int sock;
    char name[MAX_NAME];
} Client;

int send_message(int, Message *);

void handle_hello(Client clients[], Client *c, Message *msg) {
    strncpy(c->name, msg->sender, MAX_NAME - 1);

    Message out = {
        .type = MSG_SYSTEM
    };

    snprintf(out.text, MAX_TEXT, "%s joined the chat", c->name);

    for (int i = 0; i < 10; i++)
        if (clients[i].sock)
            send_message(clients[i].sock, &out);
}

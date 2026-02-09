#include "../../common/message.h"

typedef struct {
    int sock;
    char name[MAX_NAME];
} Client;

int send_message(int, Message *);

void handle_chat(Client clients[], Client *c, Message *msg) {
    Message out = {
        .type = MSG_CHAT
    };

    snprintf(out.text, MAX_TEXT, "[%s] %s", c->name, msg->text);

    for (int i = 0; i < 10; i++)
        if (clients[i].sock)
            send_message(clients[i].sock, &out);
}

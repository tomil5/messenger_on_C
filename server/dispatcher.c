#include "dispatcher.h"

void handle_hello(Client[], Client *, Message *);
void handle_chat(Client[], Client *, Message *);
void handle_command(Client[], Client *, Message *);

void dispatch(Client clients[], Client *c, Message *msg) {
    switch (msg->type) {
        case MSG_HELLO:
            handle_hello(clients, c, msg);
            break;
        case MSG_CHAT:
            handle_chat(clients, c, msg);
            break;
        case MSG_COMMAND:
            handle_command(clients, c, msg);
            break;
        default:
            break;
    }
}

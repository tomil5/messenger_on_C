#include "../../common/message.h"
#include "../config.h"
#include "../dispatcher.h"
#include <string.h>
#include <unistd.h>

int send_message(int sock, Message *msg);

void handle_chat(Client clients[], Client *c, Message *msg) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sock && clients[i].sock != c->sock) {
            send_message(clients[i].sock, msg);
        }
    }
}

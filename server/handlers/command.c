#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "../../common/message.h"
#include "../config.h"

int send_message(int sock, Message *msg); // прототип из protocol.c

void cmd_time(Client *c, Client clients[], Message *msg);
void cmd_disconnect(Client *c, Client clients[], Message *msg);

typedef struct {
    const char *name;
    void (*fn)(Client *, Client[], Message *);
} Command;

Command commands[] = {
    {"time", cmd_time},
    {"disconnect", cmd_disconnect},
    {NULL, NULL}
};

void handle_command(Client clients[], Client *c, Message *msg) {
    for (int i = 0; commands[i].name; i++) {
        if (!strcmp(msg->text, commands[i].name)) {
            commands[i].fn(c, clients, msg);
            break;
        }
    }
}



// реализация команд
void cmd_time(Client *c, Client clients[], Message *msg) {
    Message out;
    out.type = MSG_SYSTEM;
    strncpy(out.sender, "Server", MAX_NAME);
    time_t t = time(NULL);
    snprintf(out.text, MAX_TEXT, "Server time: %s", ctime(&t));
    send_message(c->sock, &out);
}

void cmd_disconnect(Client *c, Client clients[], Message *msg) {
    Message out;
    out.type = MSG_SYSTEM;
    strncpy(out.sender, "Server", MAX_NAME);
    snprintf(out.text, MAX_TEXT, "%s left the chat", c->name);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sock && clients[i].sock != c->sock) {
            send_message(clients[i].sock, &out);
        }
    }

    close(c->sock);
    c->sock = 0;
}

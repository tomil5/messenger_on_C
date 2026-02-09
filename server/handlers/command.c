#include <stddef.h>
#include "../../common/message.h"
#include "server_loop.c"

typedef struct {
    int sock;
    char name[MAX_NAME];
} Client;

typedef struct {
    const char *name;
    void (*fn)(Client *, Client[], Message *);
} Command;

Command commands[] = {
    {"time", cmd_time},
    {"disconnect", cmd_disconnect},
    {NULL, NULL}    // конец табл
};

// прототиа функции отправки сообщений
int send_message(int sock, Message *msg);

// функции команжд
void cmd_time(Client *, Client[], Message *);
void cmd_disconnect(Client *, Client[], Message *);

// диспетчер команд
void handle_command(Client clients[], Client *c, Message *msg) {
    for (int i = 0; commands[i].name; i++) {
        if (!strcmp(msg->text, commands[i].name)) {
            commands[i].fn(c, clients, msg);
            break;
        }
    }
}


// ==========================
// Реализация команд
// ==========================

// \time — отправка текущего времени клиенту
void cmd_time(Client *c, Client clients[], Message *msg) {
    Message out;
    out.type = MSG_SYSTEM;
    strncpy(out.sender, "Server", MAX_NAME - 1);

    time_t t = time(NULL);
    snprintf(out.text, MAX_TEXT, "Server time: %s", ctime(&t)); // ctime добавляет \n

    send_message(c->sock, &out);
}

// \disconnect — отключение клиента
void cmd_disconnect(Client *c, Client clients[], Message *msg) {
    Message out;
    out.type = MSG_SYSTEM;
    strncpy(out.sender, "Server", MAX_NAME - 1);

    snprintf(out.text, MAX_TEXT, "%s left the chat", c->name);

    // уведомляем остальных клиентов
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sock && clients[i].sock != c->sock) {
            send_message(clients[i].sock, &out);
        }
    }

    // закрываем сокет клиента
    close(c->sock);
    c->sock = 0;
}
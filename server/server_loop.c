#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "../common/message.h"
#include "../config.h"

typedef struct {
    int sock;
    char name[MAX_NAME];
} Client;

void dispatch(Client[], Client *, Message *);
int recv_message(int, Message *);

static int create_server(void) {
    int s;
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);
    return s;
}

void server_run(void) {
    Client clients[MAX_CLIENTS] = {0};
    fd_set fds;
    int server = create_server();

    while (1) {
        FD_ZERO(&fds);
        FD_SET(server, &fds);
        int maxfd = server;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].sock) {
                FD_SET(clients[i].sock, &fds);
                if (clients[i].sock > maxfd)
                    maxfd = clients[i].sock;
            }
        }

        select(maxfd + 1, &fds, NULL, NULL, NULL);

        if (FD_ISSET(server, &fds)) {
            int c = accept(server, NULL, NULL);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (!clients[i].sock) {
                    clients[i].sock = c;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].sock && FD_ISSET(clients[i].sock, &fds)) {
                Message msg;
                if (recv_message(clients[i].sock, &msg) <= 0) {
                    close(clients[i].sock);
                    clients[i].sock = 0;
                    continue;
                }
                dispatch(clients, &clients[i], &msg);
            }
        }
    }
}

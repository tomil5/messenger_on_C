#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "server_loop.h"
#include "config.h"
#include "dispatcher.h"  // подключаем dispatcher
#include "../common/message.h"
#include "../common/protocol.h"


#define PORT 5000

static void accept_client(int server_fd, Client clients[]);
static void fdset_build(int server_fd, Client clients[], fd_set *set, int *max_sd);

void server_start(void) {
    int server_fd;
    struct sockaddr_in addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen"); exit(1);
    }

    printf("Chat server started on port %d\n", PORT);

    Client clients[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) clients[i].sock = 0;

    fd_set readfds;

    while (1) {
        int max_sd;
        FD_ZERO(&readfds);
        fdset_build(server_fd, clients, &readfds, &max_sd);

        select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds))
            accept_client(server_fd, clients);

        // обработка сообщений
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].sock > 0 && FD_ISSET(clients[i].sock, &readfds)) {
                Message msg;
                int n = recv_message(clients[i].sock, &msg);
                if (n <= 0) {
                    printf("%s disconnected\n", clients[i].name);
                    close(clients[i].sock);
                    clients[i].sock = 0;
                } else {
                    dispatch(clients, &clients[i], &msg);
                }
            }
        }
    }
}

static void accept_client(int server_fd, Client clients[]) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int fd = accept(server_fd, (struct sockaddr*)&addr, &len);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sock == 0) {
            clients[i].sock = fd;
            strncpy(clients[i].name, "Unknown", MAX_NAME);
            Message welcome;
            welcome.type = MSG_SYSTEM;
            strncpy(welcome.sender, "Server", MAX_NAME);
            snprintf(welcome.text, MAX_TEXT, "Welcome to the chat!");
            send_message(fd, &welcome);
            return;
        }
    }

    close(fd);
}

static void fdset_build(int server_fd, Client clients[], fd_set *set, int *max_sd) {
    FD_SET(server_fd, set);
    *max_sd = server_fd;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sock > 0) {
            FD_SET(clients[i].sock, set);
            if (clients[i].sock > *max_sd)
                *max_sd = clients[i].sock;
        }
    }
}

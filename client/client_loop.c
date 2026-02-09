#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "../common/message.h"
#include "client_loop.h"

#define PORT 5000

static int connect_to_server(void);

void client_start(void) {
    int sockfd = connect_to_server();
    printf("Connected to chat. Enter your name:\n");

    char name[MAX_NAME];
    fgets(name, MAX_NAME, stdin);

    Message msg;
    msg.type = MSG_HELLO;
    strncpy(msg.sender, name, MAX_NAME);
    strncpy(msg.text, name, MAX_TEXT);
    write(sockfd, &msg, sizeof(msg));

    fd_set readfds;
    char buffer[MAX_TEXT];

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);
        int max_sd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;

        select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &readfds)) {
            int n = read(sockfd, &msg, sizeof(msg));
            if (n <= 0) {
                printf("Disconnected from server\n");
                close(sockfd);
                exit(0);
            }
            printf("%s: %s", msg.sender, msg.text);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            fgets(buffer, MAX_TEXT, stdin);
            if (buffer[0] == '\\') {  // команда
                msg.type = MSG_COMMAND;
                strncpy(msg.text, buffer + 1, MAX_TEXT);  // убираем
            } else {
                msg.type = MSG_CHAT;
                strncpy(msg.text, buffer, MAX_TEXT);
            }
            write(sockfd, &msg, sizeof(msg));
        }
    }
}

static int connect_to_server(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket"); exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton"); exit(1);
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect"); exit(1);
    }

    return sockfd;
}

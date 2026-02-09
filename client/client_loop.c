#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "../common/message.h"

void parse_input(const char *, Message *);
int send_message(int, Message *);
int recv_message(int, Message *);

#define PORT 5000

static int connect_to_server(void) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    return sock;
}

void client_start(void) {
    int sock = connect_to_server();
    fd_set fds;
    char buf[MAX_TEXT];
    Message msg;

    printf("Enter your name:\n");
    fgets(buf, sizeof(buf), stdin);

    msg.type = MSG_HELLO;
    strncpy(msg.sender, buf, MAX_NAME - 1);
    msg.sender[strcspn(msg.sender, "\n")] = '\0';
    send_message(sock, &msg);

    while (1) {
        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        FD_SET(STDIN_FILENO, &fds);

        select(sock + 1, &fds, NULL, NULL, NULL);

        if (FD_ISSET(sock, &fds)) {
            recv_message(sock, &msg);
            printf("%s\n", msg.text);
        }

        if (FD_ISSET(STDIN_FILENO, &fds)) {
            fgets(buf, sizeof(buf), stdin);
            parse_input(buf, &msg);
            send_message(sock, &msg);
        }
    }
}

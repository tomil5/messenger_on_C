#include <unistd.h>
#include <string.h>
#include "../common/message.h"

// отправка сообщения
int send_message(int sock, Message *msg) {
    return write(sock, msg, sizeof(Message));
}

// получение сообщения
int recv_message(int sock, Message *msg) {
    int n = read(sock, msg, sizeof(Message));
    return n;
}

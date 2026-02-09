#include "message.h"
#include <unistd.h>

int send_message(int sock, Message *msg) {
    return write(sock, msg, sizeof(Message));
}

int recv_message(int sock, Message *msg) {
    return read(sock, msg, sizeof(Message));
}

#include "../common/message.h"
#include <string.h>

void parse_input(const char *input, Message *msg) {
    memset(msg, 0, sizeof(*msg));

    if (input[0] == '\\') {
        msg->type = MSG_COMMAND;
        strncpy(msg->text, input + 1, MAX_TEXT - 1);
    } else {
        msg->type = MSG_CHAT;
        strncpy(msg->text, input, MAX_TEXT - 1);
    }

    msg->text[strcspn(msg->text, "\n")] = '\0';
}

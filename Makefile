CC = gcc
CFLAGS = -Wall -std=c11

COMMON_SRC = \
    common/protocol.c

SERVER_SRC = \
    server/main.c \
    server/server_loop.c \
    server/dispatcher.c \
    server/handlers/command.c \
    server/handlers/chat.c \
    server/handlers/hello.c

CLIENT_SRC = \
    client/main.c \
    client/client_loop.c

SERVER_BIN = server_app
CLIENT_BIN = client_app

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC) $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_BIN): $(CLIENT_SRC) $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

.PHONY: all clean

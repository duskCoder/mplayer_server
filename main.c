#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "mplayer_server.h"
#include "request.h"

FILE *stream_g;

static request_t buffer_g;

int callbacks_init(void);
void *get_assoc_cb(int opcode);

/* returns a socket listing to port or -1 if something failed */
static int bind_socket(uint16_t port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin;

    if (sock < 0) {
        perror("socket");
        return -1;
    }

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family      = AF_INET;
    sin.sin_port        = htons(port);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (int []){1}, sizeof(int));

    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return -1;
    }

    if (listen(sock, 10) < 0) {
        perror("listen");
        return -1;
    }

    return sock;
}

/* TODO allow multiple clients to send queries */
static int event_loop(int sock)
{
    for (;;) {
        int csock = accept(sock, NULL, NULL);

        int size = read(csock, &buffer_g, sizeof(buffer_g));

        _log("received: [%.*s]\n", size - (int)sizeof(buffer_g.opcode), buffer_g.data);

        if (size >= (int)sizeof(buffer_g.opcode)) {
            int (*cb)(const byte *, int) = get_assoc_cb(buffer_g.opcode);

            if (cb != NULL) {
                (*cb)(buffer_g.data, size - (int)sizeof(buffer_g.opcode));
            }
        }

        close(csock);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    stream_g = popen("/usr/bin/mplayer -quiet -slave -idle", "w");

    if (stream_g == NULL) {
        fprintf(stderr, "cannot run mplayer: %m\n");
        return -1;
    }

    setvbuf(stream_g, NULL, _IOLBF, BUFSIZ);

    int sock = bind_socket((argc < 2) ? 4333 : atoi(argv[1]));

    callbacks_init();

    if (sock >= 0) {
        signal(SIGPIPE, SIG_IGN);

        event_loop(sock);
    }

    pclose(stream_g);
    return 0;
}

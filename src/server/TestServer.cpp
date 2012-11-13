#include "Socket.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "THMessage.pb.h"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUF_SIZE 500

int
main(int argc, char *argv[])
{
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s  msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Socket* sock = Socket::CreateSocket(ST_TCP, "localhost", false);
    if (!sock)
    {
        printf("Failed to open socket!\n");
        return 1;
    }

    /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

    for (j = 1; j < argc; j++)
    {
        printf("Sending  msg: %d\n", j-1);

        MessagePtr msg;
        sock->Send(msg);
        msg = sock->Receive();
        printf("Received ...\n");
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}


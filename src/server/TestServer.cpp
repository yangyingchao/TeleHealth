#include "Socket.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUF_SIZE 500

int
main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s  msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = PrepareSocket("localhost", false);
    if (sock == -1)
    {
        handle_error("Failed to open socket!\n");
    }

    /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

    for (j = 2; j < argc; j++)
    {
        len = strlen(argv[j]) + 1;
        /* +1 for terminating null byte */

        if (len + 1 > BUF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }

        if (write(sfd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

        nread = read(sfd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Received %ld bytes: %s\n", (long) nread, buf);
    }

    exit(EXIT_SUCCESS);
}


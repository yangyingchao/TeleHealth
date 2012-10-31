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
        printf("Failed to open socket!\n");
    }

    /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

    for (j = 1; j < argc; j++)
    {
        printf("Sending  msg: %d\n", j-1);
        len = strlen(argv[j]) + 1;
        /* +1 for terminating null byte */

        if (len + 1 > BUF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }

        int n = write(sock, argv[j], len);
        if (n != len) {
            perror("partial/failed write\n");
            exit(EXIT_FAILURE);
        }

        nread = read(sock, buf, BUF_SIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Received %ld bytes: %s\n", (long) nread, buf);
    }

    exit(EXIT_SUCCESS);
}


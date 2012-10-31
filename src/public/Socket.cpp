#include "Socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>


#define BACK_LOG       4096

int PrepareSocket(const char* host, bool forListen)
{
    int sock = -1;

    struct addrinfo  hints;
    struct addrinfo* result = NULL;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = forListen ? AI_PASSIVE : 0;     /* For wildcard IP address */
    hints.ai_protocol = 0;           /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int ret = getaddrinfo(host, SERVER_PORT, &hints, &result);
    if (ret == 0)
    {
        struct addrinfo* rp = result;
        for (; rp != NULL; rp = rp->ai_next)
        {
            sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (sock == -1)
            {
                continue;
            }

            if (forListen)
            {
                ret = bind(sock, rp->ai_addr, rp->ai_addrlen);
                ret = ret ? ret : listen(sock, BACK_LOG);
            }
            else
            {
                ret = connect(sock, rp->ai_addr, rp->ai_addrlen);
            }

            if (ret == 0)            /* Success */
            {
                break;
            }
            else
            {
                close(sock);
            }
        }

        freeaddrinfo(result);

        if (!rp)
        {
            sock = -1;
        }
    }

    return sock;
}

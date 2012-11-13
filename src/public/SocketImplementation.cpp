#include "SocketImplementation.h"
#include "THMessage.pb.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#define BACK_LOG       4096

// Implementation of SockTcp

/* See description in header file. */
SocketTcp::SocketTcp(const char* host, bool forListen)
        : Socket()
{
    m_type = ST_TCP;

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
            m_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (m_socket == -1)
            {
                continue;
            }

            if (forListen)
            {
                ret = bind(m_socket, rp->ai_addr, rp->ai_addrlen);
                ret = ret ? ret : listen(m_socket, BACK_LOG);
            }
            else
            {
                ret = connect(m_socket, rp->ai_addr, rp->ai_addrlen);
            }

            if (ret == 0)            /* Success */
            {
                break;
            }
            else
            {
                close(m_socket);
            }
        }

        freeaddrinfo(result);

        if (!rp)
        {
            m_socket = -1;
        }
    }
}

/* See description in header file. */
SocketTcp::~SocketTcp()
{
}

/* See description in header file. */
int SocketTcp::Send(const MessagePtr& msg)
{
    return 0;
}

/* See description in header file. */
MessagePtr SocketTcp::Receive()
{
    MessagePtr msg;
    return msg;
}

/* See description in header file. */
Socket*  SocketTcp::Accept()
{
    SocketTcp* acceptedSock = NULL;
    struct sockaddr peer_addr;
    socklen_t peer_addr_size;
    peer_addr_size = sizeof(struct sockaddr);
    int clientSock = accept (m_socket,  &peer_addr,  &peer_addr_size);
    if (clientSock != -1)
    {
        // Creating
        acceptedSock = new SocketTcp();
        if (acceptedSock)
        {
            acceptedSock->m_socket = clientSock;
        }
    }

    return acceptedSock;
}

/* See description in header file. */
void SocketTcp::Close()
{
    close(m_socket);
}

/* See description in header file. */
SocketTcp::SocketTcp()
{
}

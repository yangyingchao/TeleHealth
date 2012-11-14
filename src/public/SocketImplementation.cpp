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
    TcpMessage tMsg(msg);

    while (true)
    {
        switch (tMsg.m_state)
        {
            case TMS_Invalid: // Invalid message, just return;
            {
                return -1;
            }
            case TMS_Ready: // Ready, begin to send header.
            {
                if (SendProc(&tMsg.m_header, HEADER_LENGTH))  // Finished to send Header
                {
                    tMsg.m_state = TMS_S_H;
                }
                else
                {
                    return -1;
                }
                break;
            }
            case TMS_S_H: // Header is sent, now send Body
            {
                if (SendProc(tMsg.m_body, tMsg.m_bodySize))
                {
                    tMsg.m_state = TMS_F;
                }
                else
                {
                    return -1;
                }
                break;
            }
            case TMS_F:
            {
                return 0;
            }
            default:
            {
                break;
            }
        }
    }
    return 0;
}

/* See description in header file. */
MessagePtr SocketTcp::Receive()
{
    MessagePtr msg;
    return msg;
}


/* See description in header file. */
bool SocketTcp::SendProc(void* buff, size_t length)
{
    bool result = true;
    if (buff)
    {
        size_t expected = length;
        size_t sent     = 0;
        unsigned char* ptr = (unsigned char*)buff;
        while (true)
        {
            sent = send(m_socket, ptr, expected, 0);
            if (sent != -1)
            {
                expected -= sent;
                if (!expected) // Finished Sending.
                {
                    break;
                }

                ptr+= sent;
            }
            else // Failed to send msg.
            {
                result = false;
                break;
            }
        }
    }

    return result;
}

/* See description in header file. */
bool SocketTcp::RecvProc()
{
}


/* See description in header file. */
Socket*  SocketTcp::Accept()
{

    //TODO: Check sock type before accept!!
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

/* See description in header file. */
TcpMessage::TcpMessage(MessagePtr message)
        : m_message(message),
          m_state(TMS_Ready),
          m_body(NULL)
{
    memset(&m_header, 0, sizeof(m_header));
    if (message)
    {
        m_header.data_size = message->ByteSize();
        m_bodySize = m_header.data_size;
        if (m_header.data_size)
        {
            m_body = malloc(m_bodySize);
            memset(m_body, 0, m_bodySize);

            if (m_message->SerializeToArray(m_body, m_bodySize))
            {
                if (m_body)
                {
                    free(m_body);
                }
                m_state = TMS_Invalid;
            }
        }
    }
}

/* See description in header file. */
TcpMessage::~TcpMessage()
{
    if (m_body)
    {
        free(m_body);
    }
}

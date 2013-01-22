#include "SocketImplementation.h"
#include "THMessage.pb.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include "DataBlob.h"

#define DEBUG

#include "LogUtils.h"

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
SocketTcp::SocketTcp()
{
}


// Implementation of TCP Message
/* See description in header file. */
TcpMessage::TcpMessage(THMessagePtr message)
        : m_pTHMessage(message),
          m_state(TMS_Invalid),
          m_packetHeader(NULL),
          m_packetData(NULL),
          m_dataSize(0)
{
    if (message)
    {
        DataBlobPtr headerBlob = message->GetHeaderBlob();
        DataBlobPtr bodyBlob   = message->GetBodyBlob();

        m_packetHeader = headerBlob ? headerBlob->GetData() : NULL;
        m_packetData   = bodyBlob ? bodyBlob->GetData() : NULL;
        m_dataSize     = bodyBlob ? bodyBlob->GetDataSize() : 0;

        if (m_packetHeader) // Data maybe optional...
        {
            m_state      = TMS_Ready;
        }
    }
    PDEBUG ("Header: %p, Body: %p, BodySize: %d\n",
            m_packetHeader, m_packetData, m_dataSize);
}

/* See description in header file. */
TcpMessage::~TcpMessage()
{
}

/* See description in header file. */
TcpMessage::TcpMessage()
        : m_state(TMS_Invalid),
          m_packetHeader(NULL),
          m_packetData(NULL),
          m_dataSize(0)
{
    m_pBlob = DataBlob::GetInstance();
    if (m_pBlob && m_pBlob->PrepareSpace(HEADER_LENGTH) &&
        (m_packetHeader = m_pBlob->GetData()))
    {
        m_state = TMS_Ready;
    }
}

/* See description in header file. */
bool TcpMessage::PrepareSpace(size_t size)
{
    bool ret = true;
    if (size)
    {
        m_packetData = malloc(size);
        if (m_packetData)
        {
            memset(m_packetData, 0, size);
        }
        else
        {
            ret = false;
        }
    }
    return ret;
}

/* See description in header file. */
bool TcpMessage::ParseHeader()
{
    bool ret = false;
    m_pTHMessage.reset(new THMessage);
    if (m_pTHMessage && m_pTHMessage->LoadHeaderFromBlob(m_pBlob))
    {
        DataBlobPtr bodyBlob = m_pTHMessage->GetBodyBlob();
        if (bodyBlob)
        {
            m_packetData = bodyBlob->GetData();
            m_dataSize = bodyBlob->GetDataSize();
            PDEBUG ("data: %p, size: %d\n", m_packetData, m_dataSize);
            ret = true;
        }
    }
#ifdef DEBUG
    if (!ret)
    {
        printf("%s: Returning false!\n", __FUNCTION__);
    }
#endif
    return ret;
}


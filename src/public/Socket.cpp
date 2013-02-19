#include "Socket.h"
#include "LogUtils.h"
#include "SocketTcp.h"
#include "DataBlob.h"
#include "THMessage.pb.h"
#include "SockUnix.h"

// Implementation of Socket.

/* See description in header file. */
Socket::Socket()
        : m_socket(-1),
          m_type(ST_INVALID)
{
}

/* See description in header file. */
Socket::~Socket()
{
}

/* See description in header file. */
Socket*  Socket::CreateSocket(SocketType type, const char* host,
                              const char* port, bool forListen)
{
    Socket* sock = NULL;

    switch (type)
    {
        case ST_TCP:
        {
            sock = new SocketTcp(host, port, forListen);
            if (sock->m_socket == -1)
            {
                delete sock;
                sock = NULL;
            }
            break;
        }

        case ST_UNIX:
        {
            sock = new SocketUnix(host, forListen);
            break;
        }
        // Not implemented for now.
        case ST_UDP:
        {
            break;
        }

        default:
        {
            break;
        }
    }

    return sock;
}

/* See description in header file. */
int Socket::Send(const THMessagePtr& msg)
{
    SocketMessage tMsg(msg);

    while (true)
    {
        switch (tMsg.m_state)
        {
            case TMS_Invalid: // Invalid message, just return;
            {
                goto error;
            }
            case TMS_Ready: // Ready, begin to send header.
            {
                if (!SendProc(tMsg.m_packetHeader, HEADER_LENGTH))
                {
                    goto error;
                }

                // Finished to send Header
                tMsg.m_state = TMS_S_H;
                break;
            }
            case TMS_S_H: // Header is sent, now send Body
            {
                if (SendProc(tMsg.m_packetData, tMsg.m_dataSize))
                {
                    tMsg.m_state = TMS_F;
                }
                else
                {
                    goto error;
                }
                break;
            }
            case TMS_F:
            {
                return 0;
            }
            default:
            {
                PDEBUG ("Unexpected state: %d\n", tMsg.m_state);
                break;
            }
        }
    }
    return 0;
error:
    return -1;
}

/* See description in header file. */
THMessagePtr Socket::Receive()
{
    SocketMessage tMsg;
    while (true)
    {
        switch (tMsg.m_state)
        {
            case TMS_Invalid:
            {
                PDEBUG ("1\n");
                goto error;
            }
            case TMS_Ready:
            {
                if (!RecvProc(tMsg.m_packetHeader, HEADER_LENGTH))
                {
                    PDEBUG ("2\n");
                    goto error;
                }

                tMsg.m_state = TMS_R_H;
                break;
            }
            case TMS_R_H: // Header received.
            {
                if (!tMsg.ParseHeader() || !RecvProc(tMsg.m_packetData, tMsg.m_dataSize))
                {
                    goto error;
                }

                tMsg.m_state = TMS_F;
                break;
            }
            case TMS_F:
            {
                goto ok;
            }
            default:
            {
                break;
            }
        }
    }

error:
    tMsg.m_pTHMessage.reset();
ok:
    PDEBUG ("Returning: %p\n", tMsg.m_pTHMessage.get());
    return tMsg.m_pTHMessage;
}

/* See description in header file. */
bool Socket::SendProc(void* buff, size_t length)
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
bool Socket::RecvProc(void* buff, size_t length)
{
    bool result = false;
    if (buff)
    {
        size_t expected = length;
        size_t received = 0;
        unsigned char* ptr = (unsigned char*)buff;
        while (true)
        {
            received = recv(m_socket, ptr, expected, 0);
            if (received != -1 && received != 0)
            {
                expected -= received;
                if (!expected) // Finished Sending.
                {
                    result = true;
                    break;
                }

                ptr+= received;
            }
            else // Failed to send msg.
            {
                break;
            }
        }
    }
    else if (!buff && !length)
    {
        result = true;
    }

    if (!result)
    {
        PDEBUG ("returing false!\n");
    }
    return result;
}


// Implementation of SockMessage
/* See description in header file. */
SocketMessage::SocketMessage(THMessagePtr message)
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
SocketMessage::~SocketMessage()
{
}

/* See description in header file. */
SocketMessage::SocketMessage()
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
bool SocketMessage::PrepareSpace(size_t size)
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
bool SocketMessage::ParseHeader()
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

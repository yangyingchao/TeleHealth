#include "Socket.h"

#include "SocketImplementation.h"
#include "THMessage.pb.h"

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
Socket*  Socket::CreateSocket(SocketType type, const char* host, bool forListen)
{
    Socket* sock = NULL;

    switch (type)
    {
        case ST_TCP:
        {
            sock = new SocketTcp(host, forListen);
            if (sock->m_socket == -1)
            {
                delete sock;
                sock = NULL;
            }
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
    TcpMessage tMsg(msg);

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
    TcpMessage tMsg;
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
Socket* Socket::Accept()
{
    //XXX: Implement this!
}

/* See description in header file. */
void Socket::Close()
{
    close(m_socket);
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



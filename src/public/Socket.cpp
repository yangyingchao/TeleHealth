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



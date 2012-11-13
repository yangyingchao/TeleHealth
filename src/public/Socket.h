#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "SmartPointer.h"

#define SERVER_PORT           "5678"

typedef enum _SocketType
{
    ST_INVALID = 0,
    ST_TCP,
    ST_UDP,
} SocketType;

class Message;
typedef shared_ptr<Message> MessagePtr;

class Socket
{
public:
    Socket();
    virtual ~Socket();

    // TODO: Separate into pieces.
    static Socket* CreateSocket(SocketType type = ST_TCP,
                                const char* host = NULL,
                                bool forListen = true);

    virtual int Send(const MessagePtr& msg) = 0;
    virtual MessagePtr Receive() = 0;
    virtual Socket* Accept() = 0;
    virtual void Close() = 0;

protected:
    int        m_socket;
    SocketType m_type;

};
#endif /* _SOCKET_H_ */

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

class THMessage;
typedef shared_ptr<THMessage> THMessagePtr;

class Socket
{
public:
    Socket();
    virtual ~Socket();

    // TODO: Separate into pieces.
    static Socket* CreateSocket(SocketType type = ST_TCP,
                                const char* host = NULL,
                                bool forListen = true);

    virtual int Send(const THMessagePtr& msg);
    virtual THMessagePtr Receive();
    virtual void Close();
    virtual 

protected:
    bool SendProc(void* buff, size_t length);
    bool RecvProc(void* buff, size_t length);

    int        m_socket;
    SocketType m_type;

};
#endif /* _SOCKET_H_ */

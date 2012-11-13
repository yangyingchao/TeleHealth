#ifndef _SOCKETIMPLEMENTATION_H_
#define _SOCKETIMPLEMENTATION_H_

#include "Socket.h"

class SocketTcp : public Socket
{
public:
    SocketTcp();
    SocketTcp(const char* host, bool forListen);
    virtual ~SocketTcp();

    virtual int Send(const MessagePtr& msg);

    virtual MessagePtr Receive();

    virtual Socket* Accept();

    virtual void Close();

private:
    struct sockaddr m_addr;
};


#endif /* _SOCKETIMPLEMENTATION_H_ */

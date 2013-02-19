#ifndef _SOCKETIMPLEMENTATION_H_
#define _SOCKETIMPLEMENTATION_H_

#include "Socket.h"

class SocketTcp : public Socket
{
public:
    SocketTcp();
    SocketTcp(const char* host, const char* port, bool forListen);
    virtual ~SocketTcp();

    virtual Socket* Accept();
    virtual void Close();
};

#endif /* _SOCKETIMPLEMENTATION_H_ */

#ifndef _SOCKUNIX_H_
#define _SOCKUNIX_H_

#include "Socket.h"

class SocketUnix : public Socket
{
public:
    SocketUnix(const char* host, bool forListen);
    virtual ~SocketUnix();

    virtual Socket* Accept();
    virtual void Close();
};
#endif /* _SOCKUNIX_H_ */

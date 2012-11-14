#ifndef _SOCKETIMPLEMENTATION_H_
#define _SOCKETIMPLEMENTATION_H_

#include "Socket.h"
#include "MessageBase.h"

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
    bool SendProc(void* buff, size_t length);
    bool RecvProc(void* buff, size_t length);
    struct sockaddr m_addr;
};


typedef enum _TcpMessageState
{
    TMS_Invalid = 0,                    /*!< Invalid state.    */
    TMS_Ready,                          /*!< Ready state */
    TMS_S_H,                            /*!< Header Sent   */
    TMS_S_B,                            /*!< Body Sent      */
    TMS_R_H,                            /*!< Header Received */
    TMS_R_B,                            /*!< Body Received   */
    TMS_F,                              /*!< Finished. */
} TcpMessageState;

class TcpMessage
{
public:
    friend class SocketTcp;

    TcpMessage(MessagePtr message);
    TcpMessage();
    virtual ~TcpMessage();

    bool ParseHeader();
    bool ParseBody();

protected:
    MessagePtr      m_message;
    TcpMessageState m_state;

    msg_header      m_header;
    void*           m_body;
    size_t          m_bodySize;

private:
    bool PrepareSpace(size_t size);

};

#endif /* _SOCKETIMPLEMENTATION_H_ */

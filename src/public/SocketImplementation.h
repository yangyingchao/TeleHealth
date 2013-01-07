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



typedef struct _msg_header
{
    uint8[3] msg_length;
    uint8    digest;
} msg_header;

#define HEADER_LENGTH       sizeof(msg_header)

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

    msg_header      m_tcpHeader; // Tcp Header,
    void*           m_tcpData;   // TCP Data, dumpped from Message class.
    size_t          m_dataSize;

private:
    bool PrepareSpace(size_t size);
    static uint32 m_checkSum = 0x12E72AE;
};

#endif /* _SOCKETIMPLEMENTATION_H_ */

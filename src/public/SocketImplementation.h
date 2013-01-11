#ifndef _SOCKETIMPLEMENTATION_H_
#define _SOCKETIMPLEMENTATION_H_

#include "Socket.h"
#include "THMessageBase.h"

class DataBlob;

class SocketTcp : public Socket
{
public:
    SocketTcp();
    SocketTcp(const char* host, bool forListen);
    virtual ~SocketTcp();

    virtual int Send(const THMessagePtr& msg);

    virtual THMessagePtr Receive();

    virtual Socket* Accept();

    virtual void Close();

private:
    bool SendProc(void* buff, size_t length);
    bool RecvProc(void* buff, size_t length);
    struct sockaddr m_addr;
};


typedef enum _TcpTHMessageState
{
    TMS_Invalid = 0,                    /*!< Invalid state.    */
    TMS_Ready,                          /*!< Ready state */
    TMS_S_H,                            /*!< Header Sent   */
    TMS_S_B,                            /*!< Body Sent      */
    TMS_R_H,                            /*!< Header Received */
    TMS_R_B,                            /*!< Body Received   */
    TMS_F,                              /*!< Finished. */
} TcpTHMessageState;


class TcpTHMessage
{
public:
    friend class SocketTcp;

    TcpTHMessage(THMessagePtr message);
    TcpTHMessage();
    virtual ~TcpTHMessage();

    bool ParseHeader();
    bool ParseBody();

protected:
    THMessagePtr      m_pTHMessage;
    TcpTHMessageState m_state;

    unsigned char m_header[HEADER_LENGTH]; // All message headers should dumped into this buffer,  and
    void* m_packetHeader;
    void* m_packetData;
private:
    shared_ptr<DataBlob> m_pBlob;
    bool PrepareSpace(size_t size);
    static uint32 m_checkSum = 0x12E72AE;
};

#endif /* _SOCKETIMPLEMENTATION_H_ */

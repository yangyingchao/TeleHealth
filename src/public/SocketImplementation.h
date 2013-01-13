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

    TcpMessage(THMessagePtr message);
    TcpMessage();
    virtual ~TcpMessage();

    bool ParseHeader();
    bool ParseBody();

private:
    bool PrepareSpace(size_t size);

    THMessagePtr         m_pTHMessage;

    unsigned char        m_header[HEADER_LENGTH];
    TcpMessageState      m_state;
    shared_ptr<DataBlob> m_pBlob;

    void* m_packetHeader;
    void* m_packetData;
    uint32 m_dataSize;

};

#endif /* _SOCKETIMPLEMENTATION_H_ */

#ifndef _SOCKETIMPLEMENTATION_H_
#define _SOCKETIMPLEMENTATION_H_

#include "Socket.h"
#include "MessageBase.h"

class DataBlob;

class SocketTcp : public Socket
{
public:
    SocketTcp();
    SocketTcp(const char* host, bool forListen);
    virtual ~SocketTcp();

    virtual Socket* Accept();


private:
    bool SendProc(void* buff, size_t length);
    bool RecvProc(void* buff, size_t length);
};


typedef enum _TcpMessageState
{
    TMS_Invalid = 0,                    /*!< Invalid state.  */
    TMS_Ready,                          /*!< Ready state     */
    TMS_S_H,                            /*!< Header Sent     */
    TMS_S_B,                            /*!< Body Sent       */
    TMS_R_H,                            /*!< Header Received */
    TMS_R_B,                            /*!< Body Received   */
    TMS_F,                              /*!< Finished.       */
} TcpMessageState;


class TcpMessage
{
public:
    friend class SocketTcp;

    TcpMessage(THMessagePtr message);
    TcpMessage();
    virtual ~TcpMessage();

    bool ParseHeader();

private:
    bool PrepareSpace(size_t size);

    THMessagePtr         m_pTHMessage;

    TcpMessageState      m_state;

    void*  m_packetHeader;
    void*  m_packetData;
    uint32 m_dataSize;
    DataBlobPtr m_pBlob;
};

#endif /* _SOCKETIMPLEMENTATION_H_ */

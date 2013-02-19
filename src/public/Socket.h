#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "MessageBase.h"

#define SERVER_PORT           "5678"

typedef enum _SocketType
{
    ST_TCP = 0,
    ST_UDP,
    ST_UNIX,
    ST_INVALID // Invalid too.
} SocketType;

class Socket
{
public:
    Socket();
    virtual ~Socket();

    static Socket* CreateSocket(SocketType  type = ST_TCP,
                                const char* host = NULL,
                                const char* port = SERVER_PORT,
                                bool forListen   = true);

    virtual void Close() = 0;
    virtual Socket* Accept() = 0;

    virtual int Send(const THMessagePtr& msg);
    virtual THMessagePtr Receive();

protected:
    virtual bool SendProc(void* buff, size_t length);
    virtual bool RecvProc(void* buff, size_t length);

    int        m_socket;
    SocketType m_type;

};

typedef enum _SocketMessageState
{
    TMS_Invalid = 0,                    /*!< Invalid state.  */
    TMS_Ready,                          /*!< Ready state     */
    TMS_S_H,                            /*!< Header Sent     */
    TMS_S_B,                            /*!< Body Sent       */
    TMS_R_H,                            /*!< Header Received */
    TMS_R_B,                            /*!< Body Received   */
    TMS_F,                              /*!< Finished.       */
} SocketMessageState;


class SocketMessage
{
public:
    friend class Socket;

    SocketMessage(THMessagePtr message);
    SocketMessage();
    virtual ~SocketMessage();

    bool ParseHeader();

private:
    bool PrepareSpace(size_t size);

    THMessagePtr         m_pTHMessage;

    SocketMessageState      m_state;

    void*  m_packetHeader;
    void*  m_packetData;
    uint32 m_dataSize;
    DataBlobPtr m_pBlob;
};

#endif /* _SOCKET_H_ */

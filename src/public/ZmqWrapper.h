#ifndef _ZMQWRAPPER_H_
#define _ZMQWRAPPER_H_

#include "SmartPointer.h"
#include <zmq.h>
#include "typedefs.h"

namespace google
{
namespace protobuf
{
class Message;
}
}

class ZmqContext
{
public:

    /**
     * @name ZmqContext - Constructor
     * @param threadNumber - Number of thread number.
     * @return ZmqContext
     */
    ZmqContext(int threadNumber = 1);


    /**
     * @name ZmqContext - Destructor.
     * @return void
     */
    virtual ~ZmqContext();

    /**
     * @name SetMaxSockets - sets the maximum number of sockets allowed on the context
     * @param number - Number of sockets allowed.
     * @return 0 if succeeded, or -1 otherwise.
     */
    int SetMaxSockets(int sockNumber);

    /**
     * @name get -- return real pointer of context.

      @return void*
    */
    void* get() const;

private:
    void* m_pContext;
};

typedef shared_ptr<ZmqContext> ZmqContextPtr;

/**
 * ZmqMessage -- Wrapper of zmq_msg_t and google::protocol::Message
 */

class ZmqMessage
{
public:
    /**
     * @name GetInstance - Create instance of ZmqMessage based on protobuf message.
     * @param pgMsg -  pointer of protocol buffer message instance.
     * @return shared_ptr of ZmqMessage.
     */
    static shared_ptr<ZmqMessage>
    GetInstance(const google::protobuf::Message* pbMsg=NULL);

    virtual ~ZmqMessage();

    /**
     * @name ResetFromPBMesssage - Reset zmq_msg based on pb message.
     * @param msg -  message to be seriazlied.
     * @return bool
     */
    bool ResetFromPBMesssage(const google::protobuf::Message* msg);

    /**
     * @name get - Return raw pointer of zmq_msg_t;
     * @return void*
     */
    zmq_msg_t* get() const;

    /**
     * @name data - Return raw data.
     * @return void*
     */
    void* data() const;

    /**
     * @name size - Return size of raw data.
     * @return void*
     */
    uint32 size() const;

    /**
     * @name IsEmpty - Check if a ZmqMessage is empty or not.
     * @return bool
     */
    bool IsEmpty() const;
 private:
    ZmqMessage();
    bool SerializePBMessage(const google::protobuf::Message* pbMsg);
    void Reset();
    zmq_msg_t*     m_pMsg;  /**< pointer of zmq_mst_t, used by ZmqSocket to
                               send or receive zmq_msg.  */
};

typedef shared_ptr<ZmqMessage> ZmqMessagePtr;


class ZmqSocket
{
public:
    ZmqSocket(ZmqContextPtr context, int type);
    virtual ~ZmqSocket();

    /**
     * @name IsValid - Check if this socket is valid.
     * @return bool
     */
    bool IsValid();

    /**
     * @name Send - Sends a msg
     * @param msg -  message to send.
     * @return int
     */

    int Send(ZmqMessagePtr msg);

    /**
     * @name Recv - Receives a msg.
     * @return ZmqMessagePtr
     */
    ZmqMessagePtr Recv();

    /**
     * @name Bind - Bind a to address.
     * @param addr - Character address to be connected.
     * @return 0 if succeeded.
     */
    int Bind(const char* addr);
    int Bind(const string& addr);

    /**
     * @name Connect - Connects to  an address.
     * @param addr - Character address to be connected.
     * @return 0 if succeeded.
     */
    int Connect(const char* addr);
    int Connect(const string& addr);
    void* get();
private:
    void* m_sock;
    bool m_valid;
};

typedef shared_ptr<ZmqSocket> ZmqSocketPtr;

template <typename T>
bool ZmqMsg2Message(const ZmqMessagePtr& zmsg, T& msg)
{
    return msg.ParseFromArray(zmsg->data(), zmsg->size());
}
#endif /* _ZMQWRAPPER_H_ */

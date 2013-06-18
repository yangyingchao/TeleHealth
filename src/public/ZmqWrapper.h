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
 * ZmqMessage -- Wrapper of zmq_msg_t.
 */

class ZmqMessage
{
public:
    /**
     * Default constructor.
     */
    ZmqMessage();

    /**
     * @name ZmqMessage - constructor using internal THMessage.
     * @param pMessage -  shared pointer of message instance.
     * @return ZmqMessage
     */
     // ZmqMessage(void* data, uint32 size_t size);
    ZmqMessage(shared_ptr<google::protobuf::Message> pMessage);

    virtual ~ZmqMessage();

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
private:
    void Reset();
    zmq_msg_t* m_pMsg;
};

typedef shared_ptr<ZmqMessage> ZmqMessagePtr;

#endif /* _ZMQWRAPPER_H_ */

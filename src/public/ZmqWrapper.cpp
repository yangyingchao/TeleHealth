#include "ZmqWrapper.h"
#include "generated/c/THMessage.pb.h"

// Implementation of ZmqContext.
/* See description in header file. */
ZmqContext::ZmqContext(int threadNumber)
{
    m_pContext = zmq_ctx_new();
    if (m_pContext)
    {
        if (zmq_ctx_set(m_pContext, ZMQ_IO_THREADS, threadNumber))
        {
            (void)zmq_ctx_destroy(m_pContext);
        }
    }
}

/* See description in header file. */
ZmqContext::~ZmqContext()
{
    if (m_pContext)
    {
        (void)zmq_ctx_destroy(m_pContext);
    }
}

/* See description in header file. */
int ZmqContext::SetMaxSockets(int number)
{
    return zmq_ctx_set(m_pContext, ZMQ_MAX_SOCKETS, number);
}

/* See description in header file. */
void* ZmqContext::get() const
{
    return m_pContext;
}


// Implementation of ZmqMessage
/* See description in header file. */
ZmqMessage::ZmqMessage()
{
    m_pMsg = NEW zmq_msg_t;
    if (m_pMsg && (zmq_msg_init(m_pMsg) == -1))
    {
        Reset();
    }
}

// TODO: Remove this ifdef!
#if 0
/* See description in header file. */
ZmqMessage::ZmqMessage(void* data, uint32 size_t size)
{
    m_pMsg = NEW zmq_msg_t;
    //XXX: Implement this!
}
#endif // End of #if 0

/* See description in header file. */
ZmqMessage::ZmqMessage(shared_ptr <google::protobuf::Message> pMessage)
{
    m_pMsg = NEW zmq_msg_t;
    //TODO: Init m_pMsg based on pMessage.
}

void ZmqMessage::Reset()
{
    if (m_pMsg)
    {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

/* See description in header file. */
ZmqMessage::~ZmqMessage()
{
    delete m_pMsg;
}

/* See description in header file. */
uint32 ZmqMessage::size() const
{
    return m_pMsg ? zmq_msg_size(m_pMsg) : 0;
}


/* See description in header file. */
void* ZmqMessage::data() const
{
    return m_pMsg ? zmq_msg_data(m_pMsg) : NULL;
}

/* See description in header file. */
zmq_msg_t* ZmqMessage::get() const
{
    return m_pMsg;
}


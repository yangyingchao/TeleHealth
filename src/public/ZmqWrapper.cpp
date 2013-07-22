#include "ZmqWrapper.h"
#include "generated/c/THMessage.pb.h"

void ZmqMessageDeleter(void* data, void* hint)
{
    if (data)
    {
        free(data);
    }
}

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
        : m_pbMsg(NULL)
{
    m_pMsg = NEW zmq_msg_t;
    if (m_pMsg && (zmq_msg_init(m_pMsg) == -1))
    {
        Reset();
    }
}

/* See description in header file. */
ZmqMessage::ZmqMessage(const google::protobuf::Message* pMessage)
        : m_pbMsg(pMessage)
{
    if (pMessage)
    {
        int   size = pMessage->ByteSize();
        void* data = NULL;
        if (size > 0)
        {
            data = malloc(size);
        }

        if (data)
        {
            m_pMsg = NEW zmq_msg_t;
            if (m_pMsg && pMessage->SerializeToArray(data, size))
            {
                if (zmq_msg_init_data(m_pMsg, data, size, ZmqMessageDeleter, NULL) == -1)
                {
                    free(data);
                    Reset();
                }
            }
            else
            {
                free(data);
            }
        }
    }
}

void ZmqMessage::Reset()
{
    if (m_pMsg)
    {
        zmq_msg_close(m_pMsg);
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

/* See description in header file. */
ZmqMessage::~ZmqMessage()
{
    Reset();
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

bool ZmqMessage::IsEmpty() const
{
    return (!data() && !size());
}

/* See description in header file. */
zmq_msg_t* ZmqMessage::get() const
{
    return m_pMsg;
}

/* See description in header file. */
ZmqSocket::ZmqSocket(ZmqContextPtr context, int type)
        : m_sock(NULL),
          m_valid(false)
{
    if (context && context->get())
    {
        m_sock = zmq_socket(context->get(), type);
        if (m_sock)
        {
            m_valid = true;
        }
    }
}

/* See description in header file. */
ZmqSocket::~ZmqSocket()
{
    //XXX: Implement this!
}

/* See description in header file. */
int ZmqSocket::Send(ZmqMessagePtr msg)
{
    if (msg && msg->data() && msg->size())
    {
        return zmq_msg_send(msg->get(), m_sock, 0);
    }
    return 0;
}

/* See description in header file. */
ZmqMessagePtr ZmqSocket::Recv()
{
    ZmqMessagePtr msg(new ZmqMessage);
    int ret = zmq_msg_recv(msg->get(), m_sock, 0);
    return msg;
}

/* See description in header file. */
bool ZmqSocket::IsValid()
{
    return m_valid;
}

int ZmqSocket::Bind()
{
    return 0;
}

int ZmqSocket::Connect(const char* addr)
{
    int ret = zmq_connect(m_sock, addr);
    if (ret)
    {
        m_valid = false;
    }
    return ret;
}

int ZmqSocket::Connect(const string& addr)
{
    return Connect(addr.c_str());
}

void* ZmqSocket::get()
{
    return m_sock;
}

int ZmqSocket::Bind(const char* addr)
{
    int ret = zmq_bind(m_sock, addr);
    if (ret)
    {
        m_valid = false;
    }
    return ret;
}

int ZmqSocket::Bind(const string& addr)
{
    return Bind(addr.c_str());
}


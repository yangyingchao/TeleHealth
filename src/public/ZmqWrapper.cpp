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
shared_ptr<ZmqMessage>
ZmqMessage::GetInstance(const google::protobuf::Message* pbMsg)
{
    ZmqMessagePtr msg(NEW ZmqMessage);
    zmq_msg_t* zmsg = msg ? msg->get() : NULL;
    if (!msg || !zmsg)
    {
        goto err;
    }

    if (pbMsg)
    {
        int   size = pbMsg->ByteSize();
        void* data = NULL;
        if (size > 0)
        {
            data = malloc(size);
        }

        if (data)
        {
            if (pbMsg->SerializeToArray(data, size) &&
                (zmq_msg_init_data(zmsg, data, size, ZmqMessageDeleter, NULL) != -1))
            {
                goto ret;
            }
            free(data);
        }
        goto err;
    }
    else
    {
        if (zmq_msg_init(zmsg) != -1)
        {
            goto ret;
        }
    }

err:
    msg.reset();
ret:
    return msg;
}

/* See description in header file. */
ZmqMessage::ZmqMessage()
        : m_pMsg(NEW zmq_msg_t)
{
}

bool ZmqMessage::ResetFromPBMesssage(const google::protobuf::Message* pbMsg)
{
    bool result = false;
    if (pbMsg)
    {
        if (!m_pMsg)
        {
            m_pMsg = NEW zmq_msg_t;
        }
        else
        {
            (void)zmq_msg_close(m_pMsg);
        }

        int   size = pbMsg->ByteSize();
        void* data = NULL;
        if (size > 0)
        {
            data = malloc(size);
        }

        if (data)
        {

            if (m_pMsg && pbMsg->SerializeToArray(data, size))
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
    return result;
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
    ZmqMessagePtr msg = ZmqMessage::GetInstance();
    int ret = zmq_msg_recv(msg->get(), m_sock, 0);
    return msg;
}

/* See description in header file. */
bool ZmqSocket::IsValid()
{
    return m_valid;
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


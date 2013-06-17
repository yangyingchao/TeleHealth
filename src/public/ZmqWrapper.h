#ifndef _ZMQWRAPPER_H_
#define _ZMQWRAPPER_H_

#include "SmartPointer.h"
#include <zmq.h>

using namespace std;

class ZmqContext
{
public:

    /**
     * @name ZmqContext - Constructor
     * @param threadNumber - Number of thread number.
     * @return ZmqContext
     */
    ZmqContext(int threadNumber = 1)
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

    /**
     * @name ZmqContext - 
     * @return void
     */
    virtual ~ZmqContext()
    {
        if (m_pContext)
        {
            (void)zmq_ctx_destroy(m_pContext);
        }
    }

    /**
     * @name SetMaxSockets - sets the maximum number of sockets allowed on the context
     * @param number - Number of sockets allowed.
     * @return 0 if succeeded, or -1 otherwise.
     */
    int SetMaxSockets(int number)
    {
        return zmq_ctx_set(m_pContext, ZMQ_MAX_SOCKETS, number);
    }

    void* get() const
    {
        return m_pContext;
    }

private:
    void* m_pContext;
};
#endif /* _ZMQWRAPPER_H_ */

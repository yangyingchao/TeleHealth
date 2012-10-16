#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <zmq.hpp>

class Thread
{
public:
    Thread()
    {
        m_stop = false;
    }

    virtual ~Thread() {}
    virtual bool Start() = 0;
    virtual void Stop()
    {
        m_stop = true;
    }

protected:
    bool      m_stop;
    pthread_t m_tid;
};

#endif /* _THREAD_H_ */

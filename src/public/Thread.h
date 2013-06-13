#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <signal.h>
#include "LogUtils.h"
#include "ZmqWrapper.h"

#ifdef __APPLE__
#define TID2ULONG(X) (unsigned long)((X)->__sig)
#else
#define TID2ULONG(X) (unsigned long)(X)
#endif

class Thread
{
public:
    Thread()
    {
        m_stop = false;
    }

    virtual ~Thread()
    {
    }

    virtual bool Start() = 0;

    void SetContext(ContextPtr  pContext)
    {
        m_pContext = pContext;
    }

    virtual void Stop()
    {
        m_stop = true;
        Suicide();
    }

    virtual void Suicide()
    {
        pthread_kill(m_tid, SIGKILL);
        pthread_join(m_tid, NULL);
    }

protected:
    bool      m_stop;
    pthread_t m_tid;
    ContextPtr     m_pContext;
};

#endif /* _THREAD_H_ */

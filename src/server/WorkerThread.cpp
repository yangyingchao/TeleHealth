#include "WorkerThread.h"

/* See description in header file. */
ThreadParams::ThreadParams()
{
    // XXX: Just assume new will always succeed, overwrite new operator someday.
    pthread_mutex_init(&m_lock, NULL);
    Lock();
}

/* See description in header file. */
ThreadParams::~ThreadParams()
{
}


/* See description in header file. */
int ThreadParams::Lock()
{
    return pthread_mutex_lock(&m_lock);
}

/* See description in header file. */
int ThreadParams::Unlock()
{
    PDEBUG ("%p unlocking ..\n", this);
    return pthread_mutex_unlock(&m_lock);
}



/* See description in header file. */
WorkerThread::WorkerThread()
        : Thread(),
          m_param(NULL)
{
}

/* See description in header file. */
WorkerThread::~WorkerThread()
{
}

/* See description in header file. */
bool WorkerThread::Start()
{
    bool bRet = false;
    if (m_param)
    {
        int ret = pthread_create(&m_tid, NULL,
                                 WorkerThread::StaticThreadFunction,
                                 (void*)this);
        if (ret == 0)
        {
            bRet = true;
        }
    }
    return bRet;
}

/* See description in header file. */
void*  WorkerThread::StaticThreadFunction(void* arg)
{
    WorkerThread* pThis = NULL;
    if (arg)
    {
        pThis = static_cast<WorkerThread*>(arg);
        if (pThis)
        {
            pThis->m_tid = pthread_self();
            pThis->DoRealWorks();
        }
    }
}

/* See description in header file. */
void WorkerThread::SetThreadParam(ThreadParams* param)
{
    m_param = param;
}


/* See description in header file. */
void WorkerThread::DoRealWorks()
{
    PDEBUG ("enter\n");
    while (!m_stop)
    {
        int ret = m_param->Lock();
        if (!ret)
        {
            continue;
        }
        PDEBUG ("Thread %lu is working...\n", TID2ULONG(m_tid));

        // XXX: Do something.

        // At last, lock it!
        m_param->Lock();
    }
}

#include "WorkerThread.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* See description in header file. */
ThreadParams::ThreadParams()
{
    // XXX: Just assume new will always succeed, overwrite new operator someday.
    pthread_mutex_init(&m_lock, NULL);
    pthread_cond_init(&m_cond,  NULL);
}

/* See description in header file. */
ThreadParams::~ThreadParams()
{
}


/* See description in header file. */
int ThreadParams::WaitForAction()
{
    int ret = pthread_mutex_lock(&m_lock);
    if (ret == 0)
    {
        ret = pthread_cond_wait(&m_cond, &m_lock);
        PDEBUG ("signal received ....\n");
        ret = pthread_mutex_unlock(&m_lock);
    }
    PDEBUG ("return: %d\n", ret);
    return ret;
}

/* See description in header file. */
int ThreadParams::SignalAction()
{
    PDEBUG ("%p unlocking ..\n", this);
    int ret = pthread_mutex_lock(&m_lock);
    if (ret == 0)
    {
        PDEBUG ("ok, signalling....\n");
        ret = pthread_cond_signal(&m_cond);
        pthread_mutex_unlock(&m_lock);
    }
    return ret;
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
    while (!m_stop)
    {
        int ret = m_param->WaitForAction();
        if (ret)
        {
            continue;
        }
        PDEBUG ("Thread %lu is working...\n", TID2ULONG(m_tid));

        int sock = m_param->m_sock;
        // XXX: Do something.
        // Just echo:
        PDEBUG ("sock: %d\n", sock);
        char buff[256];

        do
        {
            memset(buff, 0, 256);

            int n = read(sock, buff, 256);
            if (n == -1)
            {
                m_param->m_busy = false;
                close(sock);
                continue;
            }
            printf("%p received: %s\n", this, buff);
            write(sock, "I know", 5);
        } while (1);
    }
}

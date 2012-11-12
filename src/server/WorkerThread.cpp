#include "WorkerThread.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define CAP_THREADS       256

/* See description in header file. */
ThreadParam::ThreadParam()
{
    // XXX: Just assume new will always succeed, overwrite new operator someday.
    pthread_mutex_init(&m_lock, NULL);
    pthread_cond_init(&m_cond,  NULL);
}

/* See description in header file. */
ThreadParam::~ThreadParam()
{
}


/* See description in header file. */
int ThreadParam::WaitForAction()
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
int ThreadParam::SignalAction()
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

// Implementation of WorkerThread

/* See description in header file. */
WorkerThread::WorkerThread()
        : Thread()
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
    int ret = pthread_create(&m_tid, NULL,
                             WorkerThread::StaticThreadFunction,
                             (void*)this);
    if (ret == 0)
    {
        bRet = true;
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
void WorkerThread::DoRealWorks()
{
    while (!m_stop)
    {
        int ret = m_param.WaitForAction();
        if (ret)
        {
            continue;
        }
        PDEBUG ("Thread %lu is working...\n", TID2ULONG(m_tid));

        int sock = m_param.m_sock;
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
                m_param.m_busy = false;
                close(sock);
                if (m_pPool)
                {
                    m_pPool->ReturnThread(this);
                }
                PDEBUG ("Socket closed ...\n");
                break;
            }
            printf("%p received: %s\n", this, buff);
            write(sock, "I know", 5);
        } while (1);
    }
}

// Implementation of ThreadPool

ThreadPool* ThreadPool::m_instance = NULL;

/* See description in header file. */
ThreadPool::~ThreadPool()
{
    // TODO: Destroy objects here.
}

/* See description in header file. */
ThreadPool*  ThreadPool::GetInstance()
{
    if (!m_instance)
    {
        m_instance = new ThreadPool();
    }
    return m_instance;
}

/* See description in header file. */
ThreadParam*  ThreadPool::BorrowThread()
{
    //TODO: lock??
    ThreadParam* param= NULL;
    if (!m_freeList.empty())
    {
        WorkerThread* thread = m_freeList.top();
        param = &thread->m_param;
        param->m_busy = true;
        m_freeList.pop();
    }

    return param;
}

/* See description in header file. */
void ThreadPool::ReturnThread(WorkerThread* thread)
{
    //TODO: lock??
    if (thread)
    {
        m_freeList.push(thread);
    }
}

/* See description in header file. */
ThreadPool::ThreadPool()
{
    // TODO: Handle exception of new operator.
    m_threads = new WorkerThread[CAP_THREADS];
    if (m_threads)
    {
        WorkerThread* thread = m_threads;
        for (int i = 0; i < CAP_THREADS; ++i, ++thread)
        {
            thread->SetThreadPool(this);
            m_freeList.push(thread);
            thread->Start();
        }
    }
}

/* See description in header file. */
void WorkerThread::SetThreadPool(ThreadPool* pool)
{
    m_pPool = pool;
}

/* See description in header file. */
void ThreadPool::CleanUp()
{
}




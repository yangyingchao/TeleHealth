#include "ThreadPool.h"
#include "WorkerThread.h"

// Implementation of ThreadPool

#define CAP_THREADS       256


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
void ThreadPool::CleanUp()
{
}

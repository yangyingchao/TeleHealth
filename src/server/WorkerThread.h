#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

#include "Thread.h"
#include <stack>

using namespace std;

class ThreadParam
{
public:
    ThreadParam();
    virtual ~ThreadParam();
    int WaitForAction();
    int SignalAction();

    pthread_mutex_t m_lock;
    pthread_cond_t  m_cond;
    int  m_sock;
    bool m_busy;
    int  m_ThreadIndex;
};

class ThreadPool;

class WorkerThread : public Thread
{
public:
    friend class ThreadPool;

    WorkerThread();
    virtual ~WorkerThread();

    virtual bool Start();

protected:
    ThreadParam m_param;
    void SetThreadPool(ThreadPool* pool);

private:
    static void* StaticThreadFunction(void* arg);
    void DoRealWorks();
    ThreadPool* m_pPool;

};

class ThreadPool
{
public:
    virtual ~ThreadPool();

    static ThreadPool* GetInstance();

    ThreadParam* BorrowThread();
    void ReturnThread(WorkerThread* thread);

    void CleanUp();

private:
    ThreadPool();

    static ThreadPool* m_instance;

    stack<WorkerThread*> m_freeList;
    WorkerThread* m_threads;
};

#endif /* _WORKERTHREAD_H_ */

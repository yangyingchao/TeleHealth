#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

#include "Thread.h"

class ThreadParams
{
public:
    ThreadParams();
    virtual ~ThreadParams();
    int Lock();
    int Unlock();

private:
    pthread_mutex_t m_lock;
};


class WorkerThread : public Thread
{
public:
    WorkerThread();
    virtual ~WorkerThread();

    virtual bool Start();
    void SetThreadParam(ThreadParams* param);


private:
    static void* StaticThreadFunction(void* arg);
    void DoRealWorks();

    ThreadParams* m_param;
};

#endif /* _WORKERTHREAD_H_ */

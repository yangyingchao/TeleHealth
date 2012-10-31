#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

#include "Thread.h"

class ThreadParams
{
public:
    ThreadParams();
    virtual ~ThreadParams();
    int WaitForAction();
    int SignalAction();

    pthread_mutex_t m_lock;
    pthread_cond_t  m_cond;
    int  m_sock;
    bool m_busy;
    int  m_ThreadIndex;
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

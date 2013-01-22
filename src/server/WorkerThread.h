#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

#include "Thread.h"
#include "Socket.h"
#include "MessageProcessor.h"

using namespace std;

class ThreadPool;
class Socket;
class Message;

class ThreadParam
{
public:
    ThreadParam();
    virtual ~ThreadParam();
    int WaitForAction();
    int SignalAction();

    pthread_mutex_t m_lock;
    pthread_cond_t  m_cond;

    Socket* m_sock;
    bool    m_busy;
    int     m_ThreadIndex;
};

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

    THMessagePtr HandleRequest(const THMessagePtr& reqest);

    ThreadPool* m_pPool;
    MessageProcessorPtr m_pMessageProcessor;

};

#endif /* _WORKERTHREAD_H_ */

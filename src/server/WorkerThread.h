#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

#include "Thread.h"

#include "MessageProcessor.h"
#include "ThreadPool.h"

using namespace std;

class Message;
class ConfigParser;

class WorkerThread : public Thread
{
public:
    friend class ThreadPool<WorkerThread>;

    WorkerThread();
    virtual ~WorkerThread();
    virtual bool Start();
    void SetContext(ZmqContextPtr context);
    void SetConfig(shared_ptr<ConfigParser> config);

protected:
    void SetThreadPool(ThreadPool<WorkerThread>* pool);

private:
    static void* StaticThreadFunction(void* arg);
    void DoRealWorks();
    THMessagePtr HandleRequest(const THMessagePtr& reqest);

    ThreadPool<WorkerThread>* m_pPool;
    MessageProcessor* m_pMessageProcessor;
    ZmqContextPtr m_pContext;
    shared_ptr<ConfigParser> m_config;
};

#endif /* _WORKERTHREAD_H_ */

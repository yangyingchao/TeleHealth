#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <stack>

using namespace std;

class ThreadParam;
class WorkerThread;

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

#endif /* _THREADPOOL_H_ */

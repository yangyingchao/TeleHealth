#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "Pool.h"
using namespace std;

#define CAP_THREADS       64

class ThreadParam;

template <class T>
class ThreadPool
{
public:
    virtual ~ThreadPool()
    {
    }

    // Just use it as simple factory.
    static ThreadPool* GetInstance(int chunkSize = CAP_THREADS)
    {
        return NEW ThreadPool<T>(chunkSize);
    }

    T* BorrowThread()
    {
        return m_pObjectPool ? (T*)m_pObjectPool->GetObject() : NULL;
    }

    void ReturnThread(T* thread)
    {
        if (m_pObjectPool)
        {
            m_pObjectPool->PutObject(thread);
        }
    }

    void CleanUp()
    {
    }

private:
    ThreadPool(int chunkSize)
    {
        m_pObjectPool = ObjectPool<T>::GetPool(chunkSize,
                                               &ThreadPool::ObjectPoolCallback,
                                               this);
    }

    static bool ObjectPoolCallback(T* obj, void* data)
    {
        bool result = false;
        if (obj && data)
        {
            obj->SetThreadPool((ThreadPool*)data);
            obj->Start();
            result = true;
        }
        return  result;;
    }

    ObjectPool<T>* m_pObjectPool;
};

#endif /* _THREADPOOL_H_ */

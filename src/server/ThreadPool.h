#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "Pool.h"
#include "ZmqWrapper.h"


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
    static ThreadPool* GetInstance(ContextPtr pContext, int chunkSize = CAP_THREADS)
    {
        return NEW ThreadPool<T>(pContext, chunkSize);
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
    ThreadPool(ContextPtr pContext, int chunkSize)
            : m_pContext(pContext)
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
            obj->SetContext();
            obj->Start(m_pContext);
            result = true;
        }
        return  result;;
    }

    ObjectPool<T>* m_pObjectPool;
    ContextPtr     m_pContext;
};

#endif /* _THREADPOOL_H_ */

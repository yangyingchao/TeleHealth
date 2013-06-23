#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "Pool.h"
#include "ZmqWrapper.h"


#define CAP_THREADS       64

template <class T>
class ThreadPool
{
public:
    virtual ~ThreadPool()
    {
    }
    typedef void (*thread_create_cb)(T*, void*);

    // Just use it as simple factory.
    static ThreadPool* GetInstance(int chunkSize = CAP_THREADS,
                                   thread_create_cb cb = NULL,
                                   void* cb_param = NULL)
    {
        return NEW ThreadPool<T>(chunkSize, cb, cb_param);
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

    ThreadPool(int chunkSize = CAP_THREADS,
               thread_create_cb cb = NULL,
               void* cb_param = NULL)
            : m_cb(cb),
              m_cb_param(cb_param)
    {
        m_pObjectPool = ObjectPool<T>::GetPool(chunkSize,
                                               &ThreadPool::ObjectPoolCallback,
                                               this);
    }

private:
    static bool ObjectPoolCallback(T* obj, void* data)
    {
        bool result = false;
        ThreadPool* pool = static_cast<ThreadPool*>(data);
        if (obj && pool)
        {
            obj->SetThreadPool(pool);
            if (pool->m_cb)
            {
                (pool->m_cb)(obj, pool->m_cb_param);
            }
            obj->Start();
            result = true;
        }
        return  result;;
    }

    ObjectPool<T>*   m_pObjectPool;
    thread_create_cb m_cb;
    void*            m_cb_param;
};

#endif /* _THREADPOOL_H_ */

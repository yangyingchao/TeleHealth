#ifndef _POOL_H_
#define _POOL_H_

#include <vector>
#include <SmartPointer.h>

template <typename T>
class ObjectPool
{
public:

    static shared_ptr<T> GetPoll(const int chunkSize)
    {
        return shared_ptr<T> (new ObjectPool<T>(chunkSize));
    }

    virtual ~ObjectPool()
    {
    }

    void Extend()
    {
        for (int i = 0; i < m_chunkSize; ++i)
        {
            m_pFreeObjects.push_back(shared_ptr<T> (new T));
        }
        m_totalSize += m_chukSize;
    }

    //TODO: need locking, need to make it lock free!
    shared_ptr<T> Get()
    {
        return shared_ptr<T>();
    }

    void Put(shared_ptr<T> instance)
    {
        if (instance)
        {
            m_pFreeObjects.push_back(instance);
        }
    }

priavte:

    ObjectPool(const int chunkSize)
            : m_chukSize(chunkSize)
    {
        (void)Extend();
    }

    vector<shared_ptr<T> > m_pFreeObjects;
    int m_chukSize;
    int m_totalSize;
};

#endif /* _POOL_H_ */

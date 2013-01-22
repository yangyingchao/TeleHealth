#ifndef _POOL_H_
#define _POOL_H_

#include <vector>
#include <SmartPointer.h>

template <typename T>
class ObjectPool
{
public:

    static shared_ptr<ObjectPool<T> > GetPool(int chunkSize)
    {
        return shared_ptr<ObjectPool<T> > (new ObjectPool<T>(chunkSize));
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
        m_totalSize += m_chunkSize;
    }

    //TODO: need locking, need to make it lock free!
    shared_ptr<T> GetObject()
    {
        if (m_pFreeObjects.empty())
        {
            Extend();
        }

        shared_ptr<T> obj = m_pFreeObjects.at(0);
        m_pFreeObjects.erase(m_pFreeObjects.begin());

        return obj;
    }

    void Put(shared_ptr<T> instance)
    {
        if (instance)
        {
            m_pFreeObjects.push_back(instance);
        }
    }

private:
    ObjectPool(const int chunkSize)
            : m_chunkSize(chunkSize)
    {
        (void)Extend();
    }

    vector<shared_ptr<T> > m_pFreeObjects;
    int m_chunkSize;
    int m_totalSize;
};

#endif /* _POOL_H_ */

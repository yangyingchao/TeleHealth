#ifndef _POOL_H_
#define _POOL_H_

#include "LFList.h"

template <typename T>
class ObjectPool
{
public:
    typedef bool (*callbackFunction)(T*, void*);
    static ObjectPool<T>* GetPool(int chunkSize,
                                  callbackFunction fn = NULL,
                                  void* data = NULL,
                                  bool autoExpand=false)
    {
        return NEW ObjectPool<T>(chunkSize, fn, data, autoExpand);
    }

    virtual ~ObjectPool()
    {
        //TODO: Free all objects ...
    }


    T* GetObject()
    {
        T* obj = (T*)m_list.DeQueue();
        if (!obj && m_autoExpand)
        {
            Extend();
            return GetObject();
        }

        return obj;
    }

    void PutObject(T* instance)
    {
        if (instance)
        {
            m_list.EnQueue(instance);
        }
    }

    int GetSize()
    {
        return m_totalSize;
    }

private:
    ObjectPool(const int chunkSize, callbackFunction fn, void* data, bool autoExpand)
            : m_chunkSize(chunkSize),
              m_extending(false),
              m_cb(fn),
              m_cbData(data),
              m_autoExpand(autoExpand)
    {
        (void)Extend();
    }

    void Extend()
    {
        if (!m_extending)
        {
            m_extending = true;
            bool err = false;
            for (int i = 0; i < m_chunkSize; ++i)
            {
                T* object(NEW T);
                if (!object)
                {
                    err = true;
                    break;
                }
                if (m_cb && (err = !m_cb(object, m_cbData)))
                {
                    break;
                }
                m_list.EnQueue(object);
            }
            if (!err)
            {
                m_totalSize += m_chunkSize;
            }
        }
        m_extending = false;
    }

    int m_chunkSize;
    int m_totalSize;

    callbackFunction m_cb;
    void* m_cbData;

    bool  m_autoExpand;
    volatile bool m_extending; // Flag to indicate this pool is being extending ...

    LF_List m_list;
};

#endif /* _POOL_H_ */

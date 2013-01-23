#ifndef _POOL_H_
#define _POOL_H_


#define CAS(X, Y, Z)       __sync_bool_compare_and_swap(&(X), Y, Z)

#include <stdlib.h>

typedef struct _record
{
    struct _record* next;
    void*           data;
} record;

template <typename T>
class ObjectPool
{
public:
    typedef bool (*callbackFunction)(T*, void*);
    static ObjectPool<T>* GetPool(int chunkSize, callbackFunction fn = NULL,  void* data = NULL)
    {
        return new ObjectPool<T>(chunkSize, fn, data);
    }

    virtual ~ObjectPool()
    {
        //TODO: Free all objects ...
    }


    T* GetObject()
    {
        T* obj = (T*)DeQueue();
        if (!obj)
        {
            Extend();
            obj = (T*)DeQueue();
        }

        return obj;
    }

    void PutObject(T* instance)
    {
        if (instance)
        {
            EnQueue(instance);
        }
    }

    int GetSize()
    {
        return m_totalSize;
    }

private:
    ObjectPool(const int chunkSize, callbackFunction fn, void* data)
            : m_chunkSize(chunkSize),
              m_listHead(NULL),
              m_listTail(NULL),
              m_extending(false),
              m_cb(fn),
              m_cbData(data)
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
                T* object(new T);
                if (!object)
                {
                    err = true;
                    break;
                }
                if (m_cb && (err = !m_cb(object, m_cbData)))
                {
                    break;
                }
                EnQueue(object);
            }
            if (!err)
            {
                m_totalSize += m_chunkSize;
            }
        }
        m_extending = false;
    }


    // Refer to: http://coolshell.cn/articles/8239.html
    void EnQueue(void* obj)
    {
        record* q = new record();
        q->data = obj;
        q->next = NULL;

        if (!CAS(m_listTail, NULL, q))
        {
            record* p    = m_listTail;
            record* oldp = p;
            do {
                while (p->next != NULL)
                    p = p->next;
            } while(!CAS(p->next, NULL, q));

            CAS(m_listTail, oldp, q);
        }
        CAS(m_listHead, NULL, q);
    }

    void* DeQueue()
    {
        record* p = NULL;
        do
        {
            p = m_listHead;
            if (p->next == NULL)
            {
                return NULL;
            }
        }
        while(!CAS(m_listHead, p, p->next));
        return p->next->data;
    }

    int m_chunkSize;
    int m_totalSize;
    record* m_listHead;
    record* m_listTail;
    callbackFunction m_cb;
    void* m_cbData;
    volatile bool m_extending; // Flag to indicate this pool is being extending ...
};

#endif /* _POOL_H_ */

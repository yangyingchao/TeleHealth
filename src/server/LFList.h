#ifndef _LF_LIST_H_
#define _LF_LIST_H_

#define CAS(X, Y, Z)       __sync_bool_compare_and_swap(&(X), Y, Z)

#include <stdlib.h>

typedef struct _record
{
    struct _record* next;
    void*           data;
} record;

class LF_List
{
public:
    LF_List()
            : m_listHead(NULL),
              m_listTail(NULL)
    {}

    virtual ~LF_List()
    {}

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

private:
    record* m_listHead;
    record* m_listTail;
};

#endif /* _LF_LIST_H_ */

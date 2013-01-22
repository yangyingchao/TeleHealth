#ifndef _DATABASETHREAD_H_
#define _DATABASETHREAD_H_

#include "Thread.h"
#include "DataBase.h"

#define INPROC_DB_PORT       "inproc://dbport"

class DBThread : public Thread
{
public:
    DBThread();
    virtual ~DBThread();
    virtual bool Start();

private:
    // Private Functions.
    static void* StaticThreadFunction(void* arg);
    void DoRealWorks();

    DataBasePtr     m_pDataBase;
};
#endif /* _DATABASETHREAD_H_ */

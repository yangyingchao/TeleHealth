#ifndef _DATABASETHREAD_H_
#define _DATABASETHREAD_H_

#include "Thread.h"

#include "DataBase.h"

#define INPROC_DB_PORT       "inproc://dbport"

class DBThread : public Thread
{
public:
    DBThread(zmq::context_t* context);
    virtual ~DBThread();
    virtual bool Start();

private:
    // Private Functions.
    static void* StaticThreadFunction(void* arg);
    void StartRealThread();

    // Private member fields.
    zmq::socket_t*  m_pListenSock;
    zmq::context_t* m_pContext;

    DataBasePtr     m_pDataBase;
};
#endif /* _DATABASETHREAD_H_ */

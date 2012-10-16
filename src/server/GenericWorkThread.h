#ifndef _GENERICWORKTHREAD_H_
#define _GENERICWORKTHREAD_H_
#include "SmartPointer.h"
#include "Thread.h"
#include "THMessage.pb.h"

#define INPROC_WORK_PORT   "inproc://workerss"


class GenericWorkerThread : public Thread
{
public:
    GenericWorkerThread(zmq::context_t* context);
    virtual ~GenericWorkerThread();
    virtual bool Start();

private:
    static void* StaticThreadFunction(void* arg);
    void StartRealThread();

    shared_ptr<zmq::message_t>  ForwardToDBThread(const Message& msg);

    zmq::context_t* m_pContext;
    zmq::socket_t*  m_pDBSock;
    zmq::socket_t*  m_pListenSock;
};

#endif /* _GENERICWORKTHREAD_H_ */

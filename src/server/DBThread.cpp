#include "DBThread.h"
#include "LogUtils.h"
#include <string>
#include <iostream>
using namespace std;

const string TEST_DB_NAME = "/tmp/test.db";

DBThread::DBThread(zmq :: context_t* context) :
        Thread(),
        m_pContext(context)
{
}

DBThread::~DBThread()
{
}

// todo: Joinable or not?
bool DBThread::Start()
{
    bool bRet = false;
    int ret = pthread_create(&m_tid, NULL,
                             DBThread::StaticThreadFunction,
                             (void*)this);
    if (ret == 0)
    {
        bRet = true;
    }
    return bRet;
}

void* DBThread::StaticThreadFunction(void* arg)
{
    DBThread* pThis = NULL;
    if (arg)
    {
        pThis = static_cast<DBThread*>(arg);
        if (pThis)
        {
            pThis->StartRealThread();
        }
    }
}

void DBThread::StartRealThread()
{
    m_pDataBase = DataBaseFactory::GetInstance(DBT_Sqlite);
    if (!m_pDataBase)
    {
        PDEBUG ("Failed to create db instance.\n");
        pthread_exit(NULL);
    }
    else
    {
        bool bRet = m_pDataBase->Open(TEST_DB_NAME);
        if (!bRet)
        {
            PDEBUG ("Failed to open db.\n");
            pthread_exit(NULL);
        }
    }

    m_pListenSock = new zmq::socket_t(*m_pContext, ZMQ_REP);
    assert(m_pListenSock);
    m_pListenSock->bind(INPROC_DB_PORT);
    while (!m_stop)
    {
        zmq::message_t request;
        m_pListenSock->recv(&request);
        if (!request.size() || !request.data())
        {
            PDEBUG ("Invalid data received! Length: %lu, data: %p\n",
                    request.size(), request.data());
            continue;
        }

        const MessageHeader& header = msg.header();
        //TODO: Check header to see if it is valid.
        switch (header.type())
        {
            case RegisterUserReq:
            {
                // Should it block?
                rsp = ProcessRegisterUser(msg);
                break;
            }
            default:
            {
                rsp = GenerateResponseMessage(!error);
                break;
            }
        }

        // XXX: Received DB operation request.
        cout << "DBThread: message received from WorkerThread" << endl;
        string data("I know...");
        zmq::message_t msg((void*)data.c_str(), data.size(), NULL);
        m_pListenSock->send(msg);
    }
}


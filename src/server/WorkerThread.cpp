#include "WorkerThread.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ThreadPool.h"

#include <iostream>

#include "THMessage.pb.h"

/* See description in header file. */
ThreadParam::ThreadParam()
{
    // XXX: Just assume new will always succeed, overwrite new operator someday.
    pthread_mutex_init(&m_lock, NULL);
    pthread_cond_init(&m_cond,  NULL);
}

/* See description in header file. */
ThreadParam::~ThreadParam()
{
}


/* See description in header file. */
int ThreadParam::WaitForAction()
{
    int ret = pthread_mutex_lock(&m_lock);
    if (ret == 0)
    {
        ret = pthread_cond_wait(&m_cond, &m_lock);
        PDEBUG ("signal received ....\n");
        ret = pthread_mutex_unlock(&m_lock);
    }
    PDEBUG ("return: %d\n", ret);
    return ret;
}

/* See description in header file. */
int ThreadParam::SignalAction()
{
    PDEBUG ("%p unlocking ..\n", this);
    int ret = pthread_mutex_lock(&m_lock);
    if (ret == 0)
    {
        PDEBUG ("ok, signalling....\n");
        ret = pthread_cond_signal(&m_cond);
        pthread_mutex_unlock(&m_lock);
    }
    return ret;
}

// Implementation of WorkerThread

/* See description in header file. */
WorkerThread::WorkerThread()
        : Thread()
{
}

/* See description in header file. */
WorkerThread::~WorkerThread()
{
}


/* See description in header file. */
bool WorkerThread::Start()
{
    bool bRet = false;
    int ret = pthread_create(&m_tid, NULL,
                             WorkerThread::StaticThreadFunction,
                             (void*)this);
    if (ret == 0)
    {
        bRet = true;
    }
    return bRet;
}

/* See description in header file. */
void*  WorkerThread::StaticThreadFunction(void* arg)
{
    WorkerThread* pThis = NULL;
    if (arg)
    {
        pThis = static_cast<WorkerThread*>(arg);
        if (pThis)
        {
            pThis->m_tid = pthread_self();
            pThis->DoRealWorks();
        }
    }
}

/* See description in header file. */
void WorkerThread::DoRealWorks()
{
    while (!m_stop)
    {
        int ret = m_param.WaitForAction();
        if (ret)
        {
            continue;
        }
        // PDEBUG ("Thread %lu is working...\n", TID2ULONG(m_tid));
        PDEBUG ("Thread %p is working...\n", this);

        Socket* sock = m_param.m_sock;
        if (!sock)
        {
            continue;
        }

        while (true)
        {
            THMessagePtr msg = sock->Receive();
            if (!msg)
            {
                m_param.m_busy = false;
                sock->Close();
                if (m_pPool)
                {
                    m_pPool->ReturnThread(this);
                }
                PDEBUG ("Socket closed ...\n");
                break;
            }

            // TODO: Add real work here.
            THMessagePtr rsp = HandleRequest(msg);

            // Send it back to client;
            int n = sock->Send(rsp);
        }
    }
}

/* See description in header file. */
void WorkerThread::SetThreadPool(ThreadPool* pool)
{
    m_pPool = pool;
}

/* See description in header file. */
THMessagePtr WorkerThread::HandleRequest(const THMessagePtr& request)
{
    THMessagePtr rsp;
    MessageHeaderPtr header = request ? request->GetMessageHeader() : MessageHeaderPtr();

    // TODO: Finish error handling here.
    if (!header)
    {
        //
    }

    if (header->has_version())
    {
        //TODO: Add version checking...
        cout << "**********\t\tReceived data: "
             << header->version().c_str() << endl;
    }

    if (!m_pMessageProcessor)
    {
        m_pMessageProcessor = MessageProcessor::GetInstance(header);
    }

    PDEBUG ("Processor: %p\n", m_pMessageProcessor);

    if (m_pMessageProcessor)
    {
        rsp = m_pMessageProcessor->ProcessMessage(request);
    }
    else // TODO: reply error!
    {
        rsp.reset(new THMessage);
        rsp->SetMessageHeader(header);
        rsp->SetMessageBody(header);
    }

    return rsp;
}




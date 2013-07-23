#include "WorkerThread.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ThreadPool.h"
#include "InternalMessages.h"
#include <ConfigParser.h>

#include <iostream>

static inline THMessagePtr Zmsg2Tmsg(ZmqMessagePtr msg)
{
    return THMessagePtr();
}

static inline ZmqMessagePtr Tmsg2Zmsg(THMessagePtr msg)
{
    return ZmqMessagePtr();
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
    if (!m_pContext || !m_pContext->get())
    {
        return ;
    }

    // Here we are using raw ProtocolBuffer (PB) message, maybe need to wrap it someday.

    sleep(5);
    // Create reporter sock.
    ZmqSocket reporter(m_pContext, ZMQ_REQ);
    if (!reporter.IsValid())
    {
        handle_error("Failed to create reporter!");
    }
    reporter.Connect(m_config->GetThreadMgtAddress().c_str());

    // PDEBUG ("connnected reporter ....\n");

    // Report to leader that this worker is available.
    MgtMsg* mgtMsg = NEW MgtMsg;
    if (!mgtMsg)
    {
        abort();
    }

    mgtMsg->set_cmd(WorkerThreadAvaiable);
    mgtMsg->set_message("Hello, leader, I'm here!");

    ZmqMessagePtr report = ZmqMessage::GetInstance(mgtMsg);
    if (!report)
    {
        abort();
    }

    int ret = reporter.Send(report);
    if (ret == -1)
    {
        abort();
    }

    // PDEBUG ("sent to reporter, ret: %d\n", ret);

    report = reporter.Recv();
    if (!report)
    {
        abort();
    }
    else
    {
        // Check response from leader, if allows to work on, then connect to work sock!
        PDEBUG ("%p: Received response, now continue...\n", this);
        MgtMsg msg;
        if (msg.ParseFromArray(report->data(), report->size()))
        {
            PDEBUG ("Cmd: %d, Message: %s\n", msg.cmd(), msg.message().c_str());
        }
        else
        {
            PDEBUG ("Failed to parse message!\n");
        }

    }

    // Create work sock.

    ZmqSocket workSock(m_pContext, ZMQ_REP);
    if (!workSock.IsValid())
    {
        handle_error("Failed to create sock for THServer\n");
    }

    if (workSock.Connect(m_config->GetDealerAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", m_config->GetDealerAddress().c_str());
        handle_error("Failed to connect to dealler address");
    }


    ZmqMessagePtr msg;
    while (true)
    {
        zmq_pollitem_t items[] = {
            {reporter.get(), 0, ZMQ_POLLIN, 0},
            {workSock.get(), 0, ZMQ_POLLIN, 0}
        };

        int ret = zmq_poll(items, 2, -1);
        if (ret == -1)
        {
            break;
        }

        // Boss assigned jobs ...
        if (items[0].revents & ZMQ_POLLIN)
        {

        }

        // Workers sent report to leader ...
        if (items[1].revents & ZMQ_POLLIN)
        {
            msg = workSock.Recv();
            if (!msg || !msg->data() || !msg->size())
            {
                PDEBUG ("Invalid data received!");
                continue;
            }

            ZmqMessagePtr rsp = Tmsg2Zmsg(HandleRequest(Zmsg2Tmsg(msg)));
            PDEBUG ("Worker: message received from WorkerThread" );
            if (rsp && rsp->get() && !rsp->size())
            {
                workSock.Send(rsp);
            }
        }
    }
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

/* See description in header file. */
void WorkerThread::SetThreadPool(ThreadPool <WorkerThread>* pool)
{
    m_pPool = pool;
}

/* See description in header file. */
void WorkerThread::SetContext(ZmqContextPtr context)
{
    m_pContext = context;
}

void WorkerThread::SetConfig(shared_ptr<ConfigParser> config)
{
    m_config = config;
}

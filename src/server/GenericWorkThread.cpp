#include "GenericWorkThread.h"

#include <fstream>
#include "GenericWorkThread.h"
#include "DBThread.h"        // Provide DB Socket.
#include "THMessage.pb.h"
#include "LogUtils.h"
#include "Utils.h"
#include <tr1/memory>
#include <StringUtils.hpp>
#include <iostream>

shared_ptr<zmq::message_t>
GenerateResponseMessage(bool succeeded)
{
    shared_ptr<zmq::message_t> rsp;
    Message msg;
    MessageHeader* header = msg.mutable_header();
    if (header)
    {
        header->set_type(RegisterUserRsp);
        header->set_length(1000);
        header->set_version("version.");
        header->set_result(succeeded);
        if (succeeded)
        {
            msg.set_data("Operation Succeeded!\n");
        }
        else
        {
            msg.set_data("Operation Failed.\n");
        }

        string data;
        if (msg.SerializeToString(&data))
        {
            rsp = shared_ptr<zmq::message_t>(new zmq::message_t((void*)data.c_str(),
                                                                data.size(), NULL));
        }
    }

    return rsp;
}

GenericWorkerThread::GenericWorkerThread(zmq::context_t* context) :
        Thread()
{
    m_pContext = context;
}

GenericWorkerThread::~GenericWorkerThread()
{
}

bool GenericWorkerThread::Start()
{
    bool bRet = false;
    int ret = pthread_create(&m_tid, NULL,
                             GenericWorkerThread::StaticThreadFunction,
                             (void*)this);
    if (ret == 0)
    {
        bRet = true;
    }
    return bRet;
}

void *GenericWorkerThread::StaticThreadFunction(void* arg)
{
    GenericWorkerThread* pThis = NULL;
    if (arg)
    {
        pThis = static_cast<GenericWorkerThread*>(arg);
        if (pThis)
        {
            pThis->DoRealWorks();
        }
    }
}

void GenericWorkerThread::DoRealWorks()
{
    assert(m_pContext);
    PDEBUG ("Enter\n");

    // Create DB Socket for sending request of DB operations.
    m_pDBSock = new zmq::socket_t(*m_pContext, ZMQ_REQ);
    assert(m_pDBSock);
    m_pDBSock->connect(INPROC_DB_PORT);

    PDEBUG ("1\n");

    //Create Work Socket to get Message from Router.
    m_pListenSock = new zmq::socket_t(*m_pContext, ZMQ_REP);
    assert(m_pListenSock);
    m_pListenSock->connect(INPROC_WORK_PORT);

    PDEBUG ("2\n");
    // Following are testing code.
    char filename[64] = {'\0'};
    sprintf(filename, "/tmp/%lu.txt", TID2ULONG(m_tid));

    bool error = false;
    int counter = 0;
    while (true)
    {
        error = false;
        //  Wait for next request from client
        zmq::message_t request;
        m_pListenSock->recv (&request);
        if (!request.size() || !request.data())
        {
            PDEBUG ("Invalid data received! Length: %lu, data: %p\n",
                    request.size(), request.data());
            continue;
        }

        Message msg;
        shared_ptr<zmq::message_t> rsp;
        if (!msg.ParseFromArray(request.data(), request.size()))
        {
            PDEBUG ("Failed to parse: length: %lu, data: %p, string: %s\n",
                    request.size(), request.data(), (char*)request.data());
            memset(filename, 0, 64);
            sprintf(filename, "/tmp/%lu_%d.dump", TID2ULONG(m_tid),
                    counter);
            fstream fd(filename, fstream::out | fstream::binary);
            if (fd.is_open())
            {
                PDEBUG ("Content dumpped to file: %s\n", filename);
                fd.write((const char*)request.data(), request.size());
                fd.close();
            }
            error = true;
        }
        else
        {
            const MessageHeader& header = msg.header();
            //TODO: Check header to see if it is valid.
            switch (header.type())
            {
                case RegisterUserReq:
                {
                    // Should it block?
                    rsp = ForwardToDBThread(&request);
                    break;
                }
                default:
                {
                    rsp = GenerateResponseMessage(!error);
                    break;
                }
            }
        }

        //  Send reply back to client
    }
    pthread_exit(NULL);
}

shared_ptr <zmq::message_t>
GenericWorkerThread::ForwardToDBThread(zmq::message_t* msg)
{
    ZMessagePtr rsp;
    if (msg)
    {
        m_pDBSock->send(*msg);
        rsp.reset(new zmq::message_t);
        m_pDBSock->recv(rsp.get());
    }
    return rsp;
}


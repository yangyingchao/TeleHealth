#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"
#include <pthread.h>
#include <vector>

ZmqMessagePtr ProcessRequest(const zmq_msg_t& req)
{
    ZmqMessagePtr rep;
    return rep;
}

void* WorkerThread(void* ctxt)
{
    if (!ctxt)
    {
        return NULL;
    }

    void* sock = zmq_socket(ctxt.get(), ZMQ_REP);
    if (!sock)
    {
        handle_error("Failed to create sock for THServer\n");
    }

    if (!zmq_connect(sock, config->GetDealerAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", config->GetDealerAddress().c_str());
        handle_error("Failed to connect to dealler address");
    }

    // TODO: here we simply created one process to handle all messages, if this
    // becomes a bottleneck someday, we can create multiple threads to do the job.
    zmq_msg_t req;
    if (zmq_msg_init(&req))
    {
        handle_error("Failed to init msg\n");
    }

    while (true)
    {
        zmq_msg_recv(&req, sock, 0);

        if (!zmq_msg_data(&req) || !zmq_msg_size(&req))
        {
            PDEBUG ("Invalid data received!");
            continue;
        }

        ZmqMessagePtr rsp = ProcessDBRequest(req);

        // XXX: Received DB operation request.
        PDEBUG ("DBThread: message received from WorkerThread" );
        if (rsp && rsp->get() && !rsp->size())
        {
            zmq_msg_send(rsp->get(), sock, 0);
        }
    }
}

int main(int argc, char *argv[])
{

    OUT_STEP("Parse Configuration ...\n");
    ConfigParserPtr config = ConfigParser::GetConfigParserWithParams(argc, argv);
    if (!config)
    {
        handle_error("Failed to get configuration\n");
    }

    OUT_STEP("Preparing ZMQContext ... \n");
    ZmqContext ctxt;
    if (!ctxt.get())
    {
        handle_error("Failed to create ZmqContext!\n");
    }

    OUT_STEP("Creating worker threads");

    vector<pthread_t> tids;
    for (int i = 0; i < config->GetThreadsPerNode(); ++i)
    {
        pthread_t tid;
        if (pthread_create(&tid, NULL, WorkerThread, ) == 0)
        {
            tids.push_back(tid);
        }
        else
        {
            break;
        }
    }

//TODO: Signal handling goes here!
    return 0;
}

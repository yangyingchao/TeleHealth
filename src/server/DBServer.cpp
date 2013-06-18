#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"

// TODO: Remove this ifdef!
ZmqMessagePtr ProcessDBRequest(const zmq_msg_t& req)
{
    ZmqMessagePtr rep;
    return rep;
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

    OUT_STEP("Preparing DB sockets");

    void* dbSock = zmq_socket(ctxt.get(), ZMQ_ROUTER);
    if (!dbSock)
    {
        handle_error("Failed to create dbSock for THServer\n");
    }

    if (!zmq_bind(dbSock, config->GetDBAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", config->GetDBAddress().c_str());
        handle_error("Failed to bind for external address");
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
        zmq_msg_recv(&req, dbSock, 0);

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
            zmq_msg_send(rsp->get(), dbSock, 0);
        }
    }

    return 0;
}

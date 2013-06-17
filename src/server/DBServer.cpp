#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"
#include <pthread.h>

// TODO: Remove this ifdef!
#if 0

MessagePtr ProcessDBRequest(const message_t& req)
{
    return new message_t;
}
#endif // End of #if 0

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

    if (!zmq_bind(frontEnd, config->GetDBAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", config->GetDBAddress().c_str());
        handle_error("Failed to bind for external address");
    }

    // TODO: here we simply created one process to handle all messages, if this
    // becomes a bottleneck someday, we can create multiple threads to do the job.
// TODO: Remove this ifdef!
#if 0

while (true)
    {
        message_t request;
        dbSock.recv(&request);
        if (!request.size() || !request.data())
        {
            PDEBUG ("Invalid data received! Length: %lu, data: %p\n",
                    request.size(), request.data());
            continue;
        }

        MessagePtr rsp = ProcessDBRequest(request);

        // XXX: Received DB operation request.
        PDEBUG ("DBThread: message received from WorkerThread" );
        dbSock.send(*rsp);
    }
#endif // End of #if 0

return 0;
}

#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"
#include <unistd.h>
#include <time.h>

ZmqContextPtr gContext;
ConfigParserPtr gConfig;

int xStep = 0;

/**
 * @name WorkerNodeManagementThread - Separated thread to manage worker nodes.
 * @return void*
 */
void* WorkerNodeManagementThread(void* data)
{
    ZmqSocket reporter(gContext, ZMQ_REP);
    if (!reporter.IsValid())
    {
        handle_error("Failed to create inprocess sock!\n");
    }

    if (reporter.Bind(gConfig->GetNodeMgtAddress()) == -1)
    {
        handle_error("Failed to bind socket to in-process sock.\n");
    }

    ZmqMessagePtr msg;
    while (true)
    {
        msg = reporter.Recv();
        if (!msg->IsEmpty())
        {

            // XXX: Here goes worker nodes management code!
            PDEBUG ("Boss: message received from leaders...\n" );
            // if (rsp && rsp->get() && !rsp->size())
            // {
            //     zmq_msg_send(rsp->get(), dbSock, 0);
            // }
            reporter.Send(msg);
        }
    }

    return NULL;
}

/**
 * @name main - Entrance of server.
 *
 * It servers as a gateway, and dispatch input messages into internal address
 * speicified by config->GetDealerAddress().
 *
 * @param argc - Number of arguments
 * @param argv - Argument vector
 * @return int
 */
int main(int argc, char *argv[])
{

    OUT_STEP("Parse Configuration ...\n");
    gConfig = ConfigParser::GetConfigParserWithParams(argc, argv);
    if (!gConfig)
    {
        handle_error("Failed to get gConfiguration\n");
    }

    OUT_STEP("Preparing ZMQContext ... \n");

    gContext.reset(NEW ZmqContext);
    if (!gContext)
    {
        handle_error("Failed to create ZmqContext!\n");
    }

    OUT_STEP("Preparing sig hanlders ...\n");
    //TODO: Add signal handling here!

    OUT_STEP("Preparing Node Management thread\n");
    pthread_t tid;
    if (pthread_create(&tid, NULL, WorkerNodeManagementThread, NULL))
    {
        handle_error("Failed to create Node Management thread\n");
    }

    // Sleep 2 seconds to wait for Node Mgt thread done.
    sleep(2);

    OUT_STEP("Preparing External sockets\n");
    void* frontEnd = zmq_socket(gContext->get(), ZMQ_ROUTER);
    if (!frontEnd)
    {
        handle_error("Failed to create frontEnd for THServer\n");
    }

    if (zmq_bind(frontEnd, gConfig->GetExternalAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", gConfig->GetExternalAddress().c_str());
        handle_error("Failed to bind for external address");
    }

    OUT_STEP("Preparing backend sockets\n");
    void* backEnd = zmq_socket(gContext->get(), ZMQ_ROUTER);
    if (!backEnd)
    {
        handle_error("Failed to create backEnd for THServer");
    }

    if (zmq_bind(backEnd, gConfig->GetDealerAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", gConfig->GetDealerAddress().c_str());
        handle_error("Failed to bind for external address");
    }

    // TODO: If necessary, write our own load-balance queue here!
    OUT_STEP("Proxy sockets");
    if (!zmq_proxy(frontEnd, backEnd, NULL))
    {
        handle_error("Failed to setup proxy!");
    }

    // Should never get here!
    zmq_close (frontEnd);
    zmq_close (backEnd);
    return 0;
}

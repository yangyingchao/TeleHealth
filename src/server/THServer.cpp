#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"

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

    OUT_STEP("Preparing External sockets\n");
    void* frontEnd = zmq_socket(ctxt.get(), ZMQ_ROUTER);
    if (!frontEnd)
    {
        handle_error("Failed to create frontEnd for THServer\n");
    }

    if (!zmq_bind(frontEnd, config->GetExternalAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", config->GetExternalAddress().c_str());
        handle_error("Failed to bind for external address");
    }

    OUT_STEP("Preparing backend sockets\n");
    void* backEnd = zmq_socket(ctxt.get(), ZMQ_ROUTER);
    if (!backEnd)
    {
        handle_error("Failed to create backEnd for THServer");
    }

    if (!zmq_bind(backEnd, config->GetDealerAddress().c_str()))
    {
        fprintf(stderr, "ERROR: addr: %s\n", config->GetDealerAddress().c_str());
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

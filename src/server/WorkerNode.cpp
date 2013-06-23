#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"
#include <pthread.h>
#include <vector>
#include "WorkerThread.h"

ZmqContextPtr gContext;
ConfigParserPtr gConfig;

int xStep = 0;

static const char* NODE_INPROC_ADDR = "inproc://worker_node.inproc";
static void SetExtraParameters(WorkerThread* thread, void* param)
{
    if (thread)
    {
        thread->SetContext(gContext);
        thread->SetConfig(gConfig);
    }
}

int main(int argc, char *argv[])
{
    OUT_STEP("Parse Configuration ...\n");
    gConfig = ConfigParser::GetConfigParserWithParams(argc, argv);
    if (!gConfig)
    {
        handle_error("Failed to get configuration\n");
    }

    OUT_STEP("Preparing ZMQContext ... \n");
    gContext.reset(new ZmqContext);
    if (!gContext || !gContext->get())
    {
        handle_error("Failed to create ZmqContext!\n");
    }

    OUT_STEP("Creating inprocess socket\n");
    ZmqSocket leader(gContext, ZMQ_REP);
    if (!leader.IsValid())
    {
        handle_error("Failed to create inprocess sock!\n");
    }

    if (leader.Bind(gConfig->GetThreadMgtAddress()) == -1)
    {
        handle_error("Failed to bind socket to in-process sock.\n");
    }


    OUT_STEP("Creating sock to communicate with boss\n");
    ZmqSocket boss(gContext, ZMQ_REQ);
    if (!boss.IsValid() || boss.Connect(gConfig->GetNodeMgtAddress()) == -1)
    {
        handle_error("Failed to communicate with my boos\n");
    }

    OUT_STEP("Creating worker threads");

    ThreadPool<WorkerThread> threadPool(gConfig->GetThreadsPerNode(),
                                        SetExtraParameters, NULL);

    // Select between boss and leader..
    while (true)
    {

        zmq_pollitem_t items[] = {
            {boss.get(), 0, ZMQ_POLLIN, 0},
            {leader.get(), 0, ZMQ_POLLIN, 0}
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

        }
    }

    return 0;
}
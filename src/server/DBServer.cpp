#include <ZmqWrapper.h>
#include <LogUtils.h>
#include "ConfigParser.h"
#include "KVDB.h"

int xStep = 0;

static   KVDB* gAccountDB; // = KVDB::GetInstance(KVDB::KVDB_Fake, path);
// TODO: Remove this ifdef!
ZmqMessagePtr ProcessDBRequest(const ZmqMessagePtr& req)
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

    // Initialize account database.
    OUT_STEP("Preparing AccountManager Database\n");
    // TODO:

    // TODO: initialize other databases ....

    OUT_STEP("Preparing ZMQContext ... \n");
    ZmqContextPtr ctxt(NEW ZmqContext);
    if (!ctxt || !ctxt->get())
    {
        handle_error("Failed to create ZmqContext!\n");
    }

    OUT_STEP("Preparing DB sockets\n");

    ZmqSocket dbSock(ctxt, ZMQ_REP);
    if (!dbSock.IsValid())
    {
        handle_error("Failed to create dbSock for DbServer\n");
    }

    if (dbSock.Bind(config->GetDBAddress()) == -1)
    {
        fprintf(stderr, "ERROR: addr: %s\n", config->GetDBAddress().c_str());
        handle_error("Failed to bind for db address");
    }

    // TODO: here we simply created one process to handle all messages, if this
    // becomes a bottleneck someday, we can create multiple threads to do the job.
    while (true)
    {
        ZmqMessagePtr req = dbSock.Recv();
        // Check if req is valid!

        ZmqMessagePtr rsp = ProcessDBRequest(req);

        // XXX: Received DB operation request.
        PDEBUG ("DBThread: message received from WorkerThread\n");
        if (rsp && rsp->get() && !rsp->size())
        {
            dbSock.Send(rsp);
        }
    }

    return 0;
}

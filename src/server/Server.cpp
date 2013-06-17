#include <iostream>
#include <errno.h>
#include <ftw.h>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "LogUtils.h"
#include "DBThread.h"
#include "WorkerThread.h"
#include "ThreadPool.h"
#include "ConfigParser.h"
#include "CommandHandlerAggregator.h"

using namespace std;
using namespace tr1;

#define MAX_WORK_THREAD       64


static ThreadPool<WorkerThread>* gWorkerThreadPool = NULL;

const int IO_THREADS = 1;

void CleanupThreadPool()
{
    if (gWorkerThreadPool)
    {
        gWorkerThreadPool->CleanUp();
    }
}



void SigActionForKill(int sig)
{
    PDEBUG ("enter\n");

    CleanupThreadPool();

    sleep(10);
    exit(1);
}

void PrepareSignalHandlers()
{
    signal (SIGINT,  SigActionForKill);
}

int main (int argc, char** argv)
{
    PrepareSignalHandlers();

    OUT_STEP("Parse Configuration ...\n");
    ConfigParserPtr config = ConfigParser::GetConfigParserWithParams(argc, argv);
    if (!config)
    {
        handle_error("Failed to get configuration\n");
    }

    OUT_STEP("Gathering data from database\n");
    // TODO:
    // XXX:

    OUT_STEP("Preparing sockets ... \n");
    ContextPtr pContext(new context_t(IO_THREADS));

    socket_t clients(*pContext, ZMQ_ROUTER);
    clients.bind("tcp://*:5555");

    socket_t workers (*pContext, ZMQ_DEALER);
    workers.bind ("inproc://workers");

    OUT_STEP("Prepare ThreadPool ... \n");

    if (!gWorkerThreadPool)
    {
        gWorkerThreadPool = ThreadPool<WorkerThread>::GetInstance(pContext);
    }

    if (!gWorkerThreadPool)
    {
        handle_error("Failed to create ThreadPool!\n");
    }

    OUT_STEP ("Init Command handlers ...\n");

    if (!InitCommandHandlers())
    {
        handle_error("Failed to initialize command handlers.\n");
    }

    OUT_STEP ("Prepare listening socket ...\n");

    Socket* listenSock = Socket::CreateSocket(ST_TCP, NULL, config->GetServerPort(), true);
    if (!listenSock)
    {
        handle_error("Failed to create socket!\n");
    }

    OUT_STEP ("Accepting connections ... \n");

    while (true)
    {
        Socket* clientSock = listenSock->Accept();
        if (clientSock)
        {
            WorkerThread* thread = gWorkerThreadPool->BorrowThread();
            if (!thread || !thread->TakeOverSocket(clientSock))
            {
                clientSock->Close();
            }
        }
        else
        {
            perror("accept");
            continue;
        }
    }

    CleanupThreadPool();

    return 0;
}

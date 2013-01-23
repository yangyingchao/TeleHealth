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
#include "Socket.h"
#include "ThreadPool.h"

#include "CommandHandlerAggregator.h"

using namespace std;
using namespace tr1;

#define MAX_WORK_THREAD       64


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

static ThreadPool<WorkerThread>* gWorkerThreadPool = NULL;

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

int main ()
{
    PrepareSignalHandlers();

    PDEBUG ("1. Prepare ThreadPool ... \n");

    if (!gWorkerThreadPool)
    {
        gWorkerThreadPool = ThreadPool<WorkerThread>::GetInstance();
    }

    if (!gWorkerThreadPool)
    {
        handle_error("Failed to create ThreadPool!\n");
    }

    PDEBUG ("2. Init Command handlers ...\n");

    if (!InitCommandHandlers())
    {
        handle_error("Failed to initialize command handlers.\n");
    }

    PDEBUG ("3. Prepare listening socket ...\n");

    Socket* listenSock = Socket::CreateSocket(ST_TCP, NULL, true);
    if (!listenSock)
    {
        handle_error("Failed to create socket!\n");
    }

    PDEBUG ("4. Accepting connections ... \n");

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

#if 0
    //  Prepare our context and sockets
    zmq::context_t context (1);

    DBThread dbThread(&context);
    bool bRet = dbThread.Start();
    if (!bRet)
    {
        cout << "Failed to start db thread. " << endl;
        return -1;
    }

    PDEBUG ("DB Thread started\n");

    zmq::socket_t clients (context, ZMQ_ROUTER);
    clients.bind ("tcp://*:5555");
    zmq::socket_t workers (context, ZMQ_DEALER);
    workers.bind (INPROC_WORK_PORT);

    GenericWorkerThread* workerThreads[MAX_WORK_THREAD];
    //  Launch pool of worker threads
    for (int i = 0; i < MAX_WORK_THREAD; i++) {
        workerThreads[i] = new GenericWorkerThread(&context);
        workerThreads[i]->Start();
    }
    //  Connect work threads to client threads via a queue
    zmq::device (ZMQ_QUEUE, clients, workers);
#endif

    CleanupThreadPool();

    return 0;
}

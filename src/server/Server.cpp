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
#include "GenericWorkThread.h"
#include "WorkerThread.h"
#include <sys/socket.h>

using namespace std;
using namespace tr1;

#define MAX_WORK_THREAD       100
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

ThreadParams g_ThreadParams[MAX_WORK_THREAD];
WorkerThread g_WorkerThread[MAX_WORK_THREAD];

void CreateThreadPool()
{
    for (int i = 0; i < MAX_WORK_THREAD; ++i)
    {
        g_WorkerThread[i].SetThreadParam(&g_ThreadParams[i]);
        g_WorkerThread[i].Start();
    }
}

void CleanupThreadPool()
{
    for (int i = 0; i < MAX_WORK_THREAD; ++i)
    {
        g_WorkerThread[i].Suicide();
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

    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == -1)
    {
        handle_error("Failed to create socket.\n");
    }

    CreateThreadPool();

    sleep(1000);
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

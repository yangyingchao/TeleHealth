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
#include "Socket.h"

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
        g_ThreadParams[i].m_ThreadIndex = i;
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
    PDEBUG ("0\n");
    printf("aaa");
    fflush(stdout);

    PrepareSignalHandlers();

    PDEBUG ("1\n");

    int listenSock = PrepareSocket(NULL, true);
    if (listenSock == -1)
    {
        handle_error("Failed to create socket!\n");
    }

    PDEBUG ("2\n");

    CreateThreadPool();

    PDEBUG ("3.\n");

    struct sockaddr peer_addr;
    socklen_t peer_addr_size;
    peer_addr_size = sizeof(struct sockaddr);
    while (true)
    {
        int clientSock = accept(listenSock,  &peer_addr,  &peer_addr_size);
        if (clientSock != -1)
        {
            // Just loop to find a free thread
            // TODO: Keep all free ones into a list!
            ThreadParams* param = NULL;
            for (int i = 0; i < MAX_WORK_THREAD; ++i)
            {
                if (!g_ThreadParams[i].m_busy)
                {
                    param = &g_ThreadParams[i];
                    break;
                }
            }
            if (param)
            {
                param->m_busy = true; // TODO: use CAS;
                param->m_sock = clientSock;
                param->Unlock(); // Unlock to make thread execute.
            }
            else // TODO: Notify user that we are busy.
            {
                close(clientSock);
            }
        }
        else
        {
            perror("accept");
            continue;
        }
    }
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

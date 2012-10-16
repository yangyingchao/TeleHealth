#include <iostream>
#include <errno.h>
#include <ftw.h>
#include <unistd.h>
#include <getopt.h>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "LogUtils.h"
#include "DBThread.h"
#include "GenericWorkThread.h"

using namespace std;
using namespace tr1;

#define MAX_WORK_THREAD       100


int main ()
{
    //  Prepare our context and sockets
    zmq::context_t context (1);

    DBThread dbThread(&context);
    bool bRet = dbThread.Start();
    if (!bRet)
    {
        cout << "Failed to start db thread. " << endl;
        return -1;
    }

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
    return 0;
}

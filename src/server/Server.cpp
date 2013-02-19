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
#include "ConfigParser.h"

#include "CommandHandlerAggregator.h"

using namespace std;
using namespace tr1;

#define MAX_WORK_THREAD       64


#define handle_error(msg)                               \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#ifdef DEBUG
int xStep = 1;
#define OUT_STEP(fmt, args...)                                          \
    do {                                                                \
        const char* file = __FILE__, *ptr = strstr(file, "..");         \
        if (!ptr) ptr = file; else while (*ptr == '.' || *ptr == '\\' || *ptr == '/') ++ptr; \
        printf("%s(%d)\t-%s, Step - %d:\t", ptr, __LINE__,__FUNCTION__, xStep++); \
        printf(fmt, ##args);                                            \
    } while(0)
#else
#define OUT_STEP(fmt, ...)
#endif

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

    OUT_STEP("Prepare ThreadPool ... \n");

    if (!gWorkerThreadPool)
    {
        gWorkerThreadPool = ThreadPool<WorkerThread>::GetInstance();
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

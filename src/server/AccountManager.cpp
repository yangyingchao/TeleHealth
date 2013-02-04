#include "SmartPointer.h"
#include "AccountManager.h"
#include "AccountRequest.h"
#include "LFList.h"
#include "Pool.h"
#include <vector.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>

static const int MaxPipes   = 64;
static const char* FakePath = "/tmp/Accounts/";

static bool ShouldStop = false; // TODO: use local socket to control thread!

pthread_t g_managerThread;

LFList      g_accountRequests;
LFList      g_writablePipes;
vector<int> g_readablePipes;

typedef struct _AddPipeFunctor
{
    AddPipeFunctor(fd_set* set, int* maxFd)
            : m_set(set),
              m_maxFd(maxFd)
    {
        if (m_maxFd)
        {
            *m_maxFd = 0;
        }
    }

    void operator(int fd)
    {
        if (m_set && m_maxFd)
        {
            FD_SET(fd, m_set);
            *m_maxFd = max(*m_maxFd, fd);
        }
    }

    fd_set* m_set;
    int*    m_maxFd;
} AddPipeFunctor;


//TODO: Add a AF_UNIX socket to control this thread!
void* account_manager_thread(void* data)
{
    //todo: listen to socket, loop g_accountRequests, process, and pass back.

    KVDB* db = static_cast<KVDB*>(data);
    if (!db || g_readablePipes.empty())
    {
        return;
    }


    while (!ShouldStop)
    {
        fd_set rfds;
        FD_ZERO(&rfds);

        int maxFd = 0;
        AddPipeFunctor functor(&rfds, &maxFd);
        for_each (g_readablePipes.begin(), g_readablePipes.end(), functor);
        ++maxFd;

        /* Wait up to five seconds. */
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int ret = select(maxFd, &rfds, NULL, NULL, &tv);
        if (ret > 0)
        {
            // We are using a lock free list to store all requests, here just read and
            // consume all pipes. select() can't tell us which fd is readable, if we use
            // epoll, life will be easier...
            char buff[256];
            vector<int>::iterator iter = g_readablePipes.begin();
            vector<int>::iterator end  = g_readablePipes.end();
            for (; iter != end; ++iter)
            {
                if (FD_ISSET(*iter, &rfds))
                {
                    (void)read(*iter, buff, 256);
                }
            }

            // Now get requests one by one, and process them.
            AccountRequest* req = NULL;
            while ((req = g_accountRequests.DeQueue()) != NULL)
            {
                if (req)
                {
                    KVPair* p = &req->kv;
                    switch (req->type)
                    {
                        case ART_CREATE:
                        {
                            req->result = db->AddKVPair(p->first, &p->second);
                            break;
                        }
                        case ART_UPDATE:
                        {
                            req->result = db->UpdateValue(p->first, &p->second);
                            break;
                        }
                        case ART_GET:
                        {
                            value_entry* value = db->GetValue(p->first);
                            if (value)
                            {
                                p->second = value;
                                p->result = true;
                            }
                            break;
                        }
                        case ART_DELETE:
                        {
                            p->reset = db->DeleteRecord(p->first);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }

                    write(req->pipe[1], 'w', 1); // Notify listener.
                }
            }
        }
        else if (ret == -1)
        {
            perror("Failed to select!");
        }
        else // Just timeout, will start it again
        {
        }
    }
    return NULL;
}

// TODO:
// Load account info into memory.
bool InilializeAccountManager(const char* path)
{
    if (!path || !strlen(path))
    {
        path = FakePath;
    }

    for (int i = 0; i < MaxPipes; ++i)
    {
        int p[2];
        if (pipe(pipes) == 0)
        {
            g_readablePipes.push_back(p[0]);
            g_writablePipes.EnQueue(p[1]);
        }
    }

    KVDB* db = KVDB::GetInstance(KVDB_Fake, path);

    if (!pthread_create(&g_managerThread, NULL, account_manager_thread, db))
    {
        return true;
    }

    return false;
}


bool IsAccountExsited()
{
    bool result = false;
    return result;
}

bool IsAccountValid()
{
    bool result = false;
    return result;
}

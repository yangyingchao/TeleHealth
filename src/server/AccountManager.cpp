#include "SmartPointer.h"
#include "AccountManager.h"
#include "LFList.h"
#include "Pool.h"
#include <vector>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
#include "KVDB.h"

static const int MaxPipes   = 64;
static const char* FakePath = "/tmp/Accounts/";

static bool ShouldStop = false; // TODO: use local socket to control thread!

pthread_t g_managerThread;

LF_List      g_accountRequests;
LF_List      g_writablePipes;
vector<int> g_readablePipes;

// Functor to add a fd into select.
struct AddPipeFunctor
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

    void operator()(int fd)
    {
        if (m_set && m_maxFd)
        {
            FD_SET(fd, m_set);
            *m_maxFd = max(*m_maxFd, fd);
        }
    }

    fd_set* m_set;
    int*    m_maxFd;
};


typedef enum _account_requestType
{
    ART_CREATE = 0,
    ART_UPDATE,
    ART_GET,
    ART_DELETE,
    ART_INVALID,
} account_requestType;

struct account_request
{
    account_request() : type(ART_INVALID), result(false) {}

    account_requestType type;
    int               pipe[2];
    KVPair            kv;
    bool              result;
};


//TODO: Add a AF_UNIX socket to control this thread!
void* account_manager_thread(void* data)
{
    //todo: listen to socket, loop g_accountRequests, process, and pass back.

    KVDB* db = static_cast<KVDB*>(data);
    if (!db || g_readablePipes.empty())
    {
        return NULL;
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
            account_request* req = NULL;
            while ((req = static_cast<account_request*>(g_accountRequests.DeQueue())) != NULL)
            {
                if (req)
                {
                    KVPair* p = &req->kv;
                    switch (req->type)
                    {
                        case ART_CREATE:
                        {
                            req->result = db->AddKVPair(p->first, p->second);
                            break;
                        }
                        case ART_UPDATE:
                        {
                            req->result = db->UpdateValue(p->first, p->second);
                            break;
                        }
                        case ART_GET:
                        {
                            req->result = db->GetValue(p->first, p->second);
                            break;
                        }
                        case ART_DELETE:
                        {
                            req->result = db->DeleteKVPair(p->first);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }

                    write(req->pipe[1], "w", 1); // Notify listener.
                }
            }
        }
        else if (ret == -1)
        {
            perror("Failed to select!");
        }
        else // Just timeout, will start it again
        {
            ;
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
        if (pipe(p) == 0)
        {
            g_readablePipes.push_back(p[0]);
            g_writablePipes.EnQueue((void*)p[1]);
        }
    }

    KVDB* db = KVDB::GetInstance(KVDB::KVDB_Fake, path);

    if (!pthread_create(&g_managerThread, NULL, account_manager_thread, db))
    {
        return true;
    }

    return false;
}


bool IsAccountExsited(Account* account)
{
    bool result = false;
    return result;
}

AM_Error RegisterAccount(Account* account)
{

    return AE_OK;
}

bool IsAccountValid(Account* account)
{
    bool result = false;
    return result;
}

static const char* AM_ErrorMessages[] =
{
    "Succeeded", "Invalid Request", "Account already existed", NULL,
};

const char* AM_ErrorStringify(AM_Error errCode)
{
    return errCode < AE_MAX ? AM_ErrorMessages[(int)errCode] : NULL;
}

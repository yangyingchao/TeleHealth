#include "SmartPointer.h"
#include "AccountManager.h"
#include "Pool.h"
#include <vector>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
#include "MessageBase.h"
#include <algorithm>
#include "KVDB.h"
#include <errno.h>
#include <stdio.h>


static const int MaxPipes   = 64;
static const char* FakePath = "/tmp/Accounts/";
static int* gPipes = NULL;

static bool ShouldStop = false; // TODO: use local socket to control thread!

pthread_t g_managerThread;

LF_List      g_accountRequests;

// MessageProcessor will write to g_writeablePipes, and account_manager_thread will get
// notified through reading the read-end.
LF_List      g_writablePipes;
vector<int*> g_readablePipes;

class Pipe
{
public:
    Pipe()
    {
        if (pipe(m_pipe))
        {
            memset(m_pipe, -1, sizeof(int)*2);
        };
    }
    ~Pipe(){(void)close(m_pipe[0]);(void)close(m_pipe[1]);}

    int GetWriteFd() {return m_pipe[1];}
    int GetReadFd() {return m_pipe[0];}
    int m_pipe[2];
};

static  ObjectPool<Pipe>* g_requestPipes = NULL; // Pipes used by each AccountRequest;


#define NOTIFY_FD(X) do { write((X), "w", 1); } while (0)
#define READ_FD(X)       do                     \
    {                                           \
        char buff[2];                           \
        (void)read((X), buff, 1);               \
    } while (0)

#define INT2PTR(X)   ((void*)(X))
#define PTR2INT(X)   ((int*)(*X))


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

    void operator()(int* fd)
    {
        if (m_set && m_maxFd)
        {
            FD_SET(*fd, m_set);
            *m_maxFd = max(*m_maxFd, *fd);
        }
    }

    fd_set* m_set;
    int*    m_maxFd;
};


typedef enum _AccountRequestType
{
    ART_INVALID = 0,
    ART_CREATE,
    ART_UPDATE,
    ART_GET,
    ART_DELETE,
    ART_MAX,
} AccountRequestType;

static inline bool IsAccountValid(Account* account)
{
    return  (account && account->has_email() && !account->email().empty()
             && account->has_passwd() && !account->passwd().empty());
}

class AccountRequest
{
public:
    AccountRequestType m_type;
    Pipe*              m_pipe;          // Will be written by manager_thread.
    KVPair             m_kv;
    bool               m_result;
    Account*           m_account;

    bool FeedKVPair()
    {
        m_kv.first = m_account->email();
        return m_account->SerializeToString(&m_kv.second);
    }

    ~AccountRequest()
    {
        if (m_pipe)
        {
            g_requestPipes->PutObject(m_pipe);
        }
    }

    static AccountRequest* GetInstance(Account* account = NULL,
                                       Pipe* p = NULL,
                                       AccountRequestType type = ART_INVALID)
    {
        AccountRequest* request = NULL;
        if (account && IsAccountValid(account) && type < ART_MAX)
        {
            request = NEW AccountRequest(account, type, p);
        }
        return request;
    }

private:
    AccountRequest(){}
    AccountRequest(Account* account, AccountRequestType type, Pipe* p)
            : m_account(account), m_type(type), m_pipe(p), m_result(false)
    {
        m_kv.first = account->email();
    }
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
            vector<int*>::iterator iter = g_readablePipes.begin();
            vector<int*>::iterator end  = g_readablePipes.end();
            for (; iter != end; ++iter)
            {
                if (FD_ISSET(**iter, &rfds))
                {
                    (void)read(**iter, buff, 256);
                }
            }

            // Now get requests one by one, and process them.
            AccountRequest* req = NULL;
            while ((req = static_cast<AccountRequest*>(g_accountRequests.DeQueue())) != NULL)
            {
                if (req)
                {
                    KVPair* p = &req->m_kv;
                    switch (req->m_type)
                    {
                        case ART_CREATE:
                        {
                            req->m_result = db->AddKVPair(p->first, p->second);
                            break;
                        }
                        case ART_UPDATE:
                        {
                            req->m_result = db->UpdateValue(p->first, p->second);
                            break;
                        }
                        case ART_GET:
                        {
                            req->m_result = db->GetValue(p->first, p->second);
                            break;
                        }
                        case ART_DELETE:
                        {
                            req->m_result = db->DeleteKVPair(p->first);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }

                    NOTIFY_FD(req->m_pipe->GetWriteFd());
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

// Load account info into memory.
bool InitializeAccountManager(const char* path)
{
    if (!path || !strlen(path))
    {
        path = FakePath;
    }

    // Create global pipes used by each requests.
    g_requestPipes = ObjectPool<Pipe>::GetPool(MaxPipes);
    if (!g_requestPipes)
    {
        return false;
    }

    // Create pipes used to communicate between threads.
    gPipes = NEW int[MaxPipes * 2];
    if (!gPipes)
    {
        return false;
    }
    int* p = gPipes;
    for (int i = 0; i < MaxPipes; ++i)
    {
        if (pipe(p) == 0)
        {
            g_readablePipes.push_back(p++);
            g_writablePipes.EnQueue(p++);
        }
    }

    // Initialize database.
    KVDB* db = KVDB::GetInstance(KVDB::KVDB_Fake, path);

    // initialize manager thread.
    if (!pthread_create(&g_managerThread, NULL, account_manager_thread, db))
    {
        return true;
    }

    return false;
}


void DestroyAccountManager()
{
    // DO clean ups....
}


static inline void WaitForRequestTBD(AccountRequest* req)
{
    // Caller should make sure req is valid.
    g_accountRequests.EnQueue(req);
    int* p = (int*)g_writablePipes.DeQueue();
    NOTIFY_FD(*p);
    READ_FD(req->m_pipe->GetReadFd());
    g_writablePipes.EnQueue(p);
}

bool IsAccountExsited(AccountRequest* req)
{
    bool result = false;
    if (req)
    {
        req->m_type = ART_GET;
        WaitForRequestTBD(req);
        result = !req->m_result;
    }
    return result;
}

// Public interfaces begins here.
AM_Error RegisterAccount(Account* account)
{
    if (!IsAccountValid(account))
    {
        return AE_INVALID;
    }

    Pipe* p = (Pipe*)g_requestPipes->GetObject();
    if (!p)
    {
        return AE_BUSY;
    }

    AM_Error err = AE_OK;
    AccountRequest* req = AccountRequest::GetInstance(account, p);
    if (!req)
    {
        err = AE_INVALID;
        goto ret;
    }

    if (IsAccountExsited(req))
    {
        err = AE_EXISTED;
        goto ret;
    }

    if (req->FeedKVPair())
    {
        req->m_type = ART_CREATE;
        req->m_account->set_status(AS_LoggedIn);
        WaitForRequestTBD(req);
        if (req->m_result)
        {
            err = AE_OK;
        }
        else
        {
            err = AE_DB;
        }
    }

ret:
    if (req)
    {
        delete req;
    }

    return err;
}


static const char* AM_ErrorMessages[] =
{
    "Succeeded", "Invalid Request", "Account already existed", "Out of memory",
    "Busy", "Database error", NULL,
};

const char* AM_ErrorStringify(AM_Error errCode)
{
    return errCode < AE_MAX ? AM_ErrorMessages[(int)errCode] : NULL;
}


AM_Error AccountLogoff(Account* account)
{
    return AE_OK;
}

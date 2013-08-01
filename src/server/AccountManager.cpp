
// TODO: Remove this ifdef!
#if 0
#include "AccountManager.h"
#include "KVDB.h"
#include "Pool.h"
#include "SmartPointer.h"
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include "MessageProcessor.h"

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
AM_Error RegisterAccount(Account* account, MsgPrivate& priv)
{
    if (!IsAccountValid(account))
    {
        return EC_INVALID_ARG;
    }

    AM_Error err = EC_OK;

    if (IsAccountExsited(req))
    {
        err = EC_EXISTED;
        goto ret;
    }

    if (req->FeedKVPair())
    {
        req->m_type = ART_CREATE;
        req->m_account->set_status(AS_LoggedIn);
        WaitForRequestTBD(req);
        if (req->m_result)
        {
            err = EC_OK;
        }
        else
        {
            err = EC_DB_ERR;
        }
    }

ret:
    if (req)
    {
        delete req;
    }

    return err;
}

/*! Handle logoff.

  @return AM_Error
*/
AM_Error AccountLogoff(Account* account)
{
    // TODO: Clean up resources, update status ....
    return EC_OK;
}
#endif // End of #if 0


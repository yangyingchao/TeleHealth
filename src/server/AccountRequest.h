#ifndef _ACCOUNTREQUEST_H_
#define _ACCOUNTREQUEST_H_

#include "typedefs.h"
typedef struct _kv_entry kv_entry;

typedef enum _account_requestType
{
    ART_CREATE = 0,
    ART_UPDATE,
    ART_GET,
    ART_DELETE,
    ART_INVALID,
} account_requestType;

//            key,     value ptr
typedef pair<uint32, kv_entry*> KVPair;


struct account_request
{
    account_request() : type(ART_INVALID), result(false) {}

    account_requestType type;
    int               pipe[2];
    KVPair            kv;
    bool              result;
};

#endif /* _ACCOUNTREQUEST_H_ */

#ifndef _ACCOUNTREQUEST_H_
#define _ACCOUNTREQUEST_H_

typedef kv_entry;

typedef enum _AcountRequestType
{
    ART_CREATE = 0,
    ART_UPDATE,
    ART_GET,
    ART_DELETE,
    ART_INVALID,
} AcountRequestType;

//            key,     value ptr
typedef pair<uint32, value_entry*> KVPair;


struct account_request
{
    account_request() : type(ART_INVALID), result(false) {}

    AcountRequestType type;
    int               pipe[2];
    KVPair            kv;
    bool              result;
};

#endif /* _ACCOUNTREQUEST_H_ */

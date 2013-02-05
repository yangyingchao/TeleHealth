#ifndef _KVDB_H_
#define _KVDB_H_

#include "typedefs.h"


typedef struct _kv_entry
{
    uint32 size;
    uint8  data[0];
} kv_entry;


class KVDB
{
public:
    virtual ~KVDB(){}

    typedef enum _KVDB_Type
    {
        KVDB_Fake = 0,
        KVDB_Invalid,
    } KVDB_Type;

    virtual bool DeleteRecord(uint32 key) = 0;
    virtual kv_entry* GetValue(uint32 key) = 0;
    virtual bool UpdateValue(uint32 key, kv_entry* value) = 0;
    virtual bool AddKVPair(uint32 key, kv_entry* value) = 0;

    static KVDB* GetInstance(KVDB_Type type, const char* path);
};

#endif /* _KVDB_H_ */

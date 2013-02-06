#ifndef _KVDBIMPLLEVELDB_H_
#define _KVDBIMPLLEVELDB_H_

#include "KVDB.h"
#include "leveldb/db.h"

class leveldb::DB;

class LevelDB : public KVDB
{
public:
    LevelDB(const char* name);
    virtual ~LevelDB();

    virtual bool AddKVPair(const string& key, const string& value);
    virtual bool DeleteKVPair(const string& key);
    virtual bool GetValue(const string& key, string& value);
    virtual bool UpdateValue(const string& key, const string& value);

private:

    bool AddKVPairInternal(const string& key, const string& value);
    string          m_dbName;
    leveldb::DB*    m_db;
    leveldb::Status m_status;

    leveldb::WriteOptions m_writeOption;
    leveldb::ReadOptions  m_readOption;
};

#endif /* _KVDBIMPLLEVELDB_H_ */

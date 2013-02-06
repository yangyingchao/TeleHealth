#include "KVDBImplLevelDB.h"
#include "leveldb/write_batch.h"
#include <assert.h>

using namespace leveldb;

/* See description in header file. */
LevelDB::LevelDB(const char* name)
        : m_dbName(name), // Caller should make sure name is not NULL
          m_db(NULL)
{
    Options options;
    options.create_if_missing = true;
    m_status = leveldb::DB::Open(options, m_dbName, &m_db);
    assert(m_status.ok());
}

/* See description in header file. */
LevelDB::~LevelDB()
{
    delete m_db;
}

/* See description in header file. */
bool LevelDB::AddKVPair(const string& key, const string& value)
{
    return (key.empty() || value.empty()) ? false : m_db->Put(m_writeOption, key, value).ok();
}

/* See description in header file. */
bool LevelDB::DeleteRecord(const string& key)
{
    return key.empty() ? false : m_db->Delete(m_writeOption, key).ok();
}

/* See description in header file. */
bool LevelDB::GetValue(const string& key, string& value)
{
    return key.empty() ? false : m_db->Get(m_readOption, key, &value).ok();
}

/* See description in header file. */
bool LevelDB::UpdateValue(const string& key, const string& value)
{
    bool result = false;
    string value1;
    if (GetValue(key, value1))
    {
        result = AddKVPair(key, value);
    }
    return result;
}

#ifndef _KVDB_H_
#define _KVDB_H_

#include <string>

using namespace std;

//            key,     value
// Value would be a string which is dumped from google protocol buffer.
typedef pair<string, string> KVPair;

class KVDB
{
public:
    virtual ~KVDB(){}

    typedef enum _KVDB_Type
    {
        KVDB_Fake = 0,
        KVDB_LevelDb,
        KVDB_Invalid,
    } KVDB_Type;

    virtual bool AddKVPair(const string& key, const string& value)   = 0;
    virtual bool DeleteKVPair(const string& key)                     = 0;
    virtual bool GetValue(const string& key, string& value)          = 0;
    virtual bool UpdateValue(const string& key, const string& value) = 0;

    static KVDB* GetInstance(KVDB_Type type, const char* path);
};

#endif /* _KVDB_H_ */

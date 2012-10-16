#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <tr1/memory>
#include <string>
#include <errno.h>

#include "DBTables.pb.h"


using namespace std;
using namespace tr1;

typedef int DB_Error;

#define DB_OK    0  // OK.
#define DB_INVAL 1  // Invalid arguments


class DataBase
{
public:
    DataBase();
    virtual ~DataBase();

    virtual DB_Error Open(const string& path) = 0;
    virtual bool Close() = 0;
    virtual DB_Error CreateTable(const string& name) = 0;
    virtual DB_Error AddUser(shared_ptr<User> user) = 0;

    typedef enum _O_FLAG
    {
        O_CREATE = 0x01,
    } O_FLAG;

};

typedef shared_ptr<DataBase> DataBasePtr;

typedef enum _DataBaseType
{
    DBT_Invalid = 0,
    DBT_Sqlite,
} DataBaseType;

class DataBaseFactory
{
public:
    static
    DataBasePtr GetInstance(DataBaseType type);

private:
    DataBaseFactory() {};
    virtual ~DataBaseFactory() {};
};



#endif /* _DATABASE_H_ */

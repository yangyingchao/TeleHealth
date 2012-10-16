#ifndef _DATABASESQLITE_H_
#define _DATABASESQLITE_H_

#include <DataBase.h>
#include <sqlite3.h>
#include "TableComponent.h"
#include "units.h"

class SqliteCommand;

class DataBaseSqlite : public DataBase
{
public:
    DataBaseSqlite();
    virtual ~DataBaseSqlite();
    virtual DB_Error Open(const string& path);
    virtual bool Close();
    virtual DB_Error CreateTable(const string& name);
    virtual DB_Error AddUser(shared_ptr<User> user);

private:

    friend class SqliteCommand;

    TableComponent* m_pTableComponent;
    std::string     m_dbPath;
    sqlite3*        m_pDb;
    std::string     m_lastErorMsg;
};

#define SQL_INDEX_INVAL       0


class SqliteCommand
{
public:
    SqliteCommand(DataBaseSqlite* db);
    SqliteCommand(DataBaseSqlite* db, const string& sql);
    SqliteCommand(DataBaseSqlite* db, const char* sql);
    virtual ~SqliteCommand();

    DB_Error Prepare(const char* sql);
    DB_Error Prepare(const string& sql);

    DB_Error Execute();

    DB_Error Finalize();

    DB_Error Bind(const char* name, int data);
    DB_Error Bind(const char* name, int64 data);
    DB_Error Bind(const char* name, double data);
    DB_Error Bind(const char* name, const char* data);
    DB_Error Bind(const char* name, const string& data);
    DB_Error Bind(const char* name, const void* data, int dataLength);

    DB_Error Bind(const string& name, int data);
    DB_Error Bind(const string& name, double data);
    DB_Error Bind(const string& name, const char* data);
    DB_Error Bind(const string& name, const string& data);
    DB_Error Bind(const string& name, const void* data, int dataLength);

private:

    int FindParameterIndex(const string& name);
    int FindParameterIndex(const char* name);

    sqlite3_stmt*   m_stmt;
    DataBaseSqlite* m_db;
};
#endif /* _DATABASESQLITE_H_ */

#include "DataBaseSqlite.h"

#include "DataBaseSqlite.h"
#include <string.h>
#include "TableComponent.h"
#include "SqliteKeywords.h"

#include <LogUtils.h>
// Mapping of sqlite keyword and EntryType.

static const char* g_SqlteKeywordMapping[] =
{
    "NULL",
    "TEXT",
    "INTEGER",
    "INTEGER",
    "INTEGER",
    "INTEGER",
    "REAL",
    "REAL",
};


// Implementation of DatabaseSqlite.

DataBaseSqlite::DataBaseSqlite()
{
    m_pTableComponent = TableComponent::GetInstance();
}

DataBaseSqlite::~DataBaseSqlite()
{
    Close();
}

DB_Error DataBaseSqlite::Open(const string & path)
{
    PDEBUG ("enter, path: %s\n", path.c_str());
    DB_Error ret = DB_OK;
    m_dbPath = path;
    if (!path.empty())
    {
        ret = sqlite3_open(path.c_str(), &m_pDb);
        if (ret != SQLITE_OK)
        {
            m_lastErorMsg = string(sqlite3_errmsg(m_pDb));
        }
    }
    PDEBUG ("return with: %d\n", ret);
    return ret;
}

bool DataBaseSqlite::Close()
{
    bool bRet = true;
    if (m_pDb && sqlite3_close(m_pDb) != SQLITE_OK)
    {
        bRet = false;
    }
    return bRet;
}

DB_Error DataBaseSqlite::CreateTable(const string& name)
{
    DB_Error ret = DB_INVAL;


    EntryList entries = m_pTableComponent->GetTableFormat(name);
    PDEBUG ("entry size: %lu\n", entries.size());
    if (!entries.empty())
    {
        string sql(CREATE_TABLE);
        sql += IFNEXT + name + LPARENT;
        EntryList::iterator iter = entries.begin();
        EntryList::iterator end  = entries.end();
        for (; iter != end;)
        {
            sql += iter->name + " " +
                   g_SqlteKeywordMapping[iter->type];

            if (iter->primary)
            {
                sql += PRIMARY_KEY;
            }

            if (++iter != end)
            {
                sql += ", ";
            }
        }
        sql += string(RPARENT) + SEMI;

        SqliteCommand cmd(this, sql);
        ret = cmd.Execute();
    }


    PDEBUG ("Create %s, ret: %d\n", name.c_str(), ret);
    return ret;
}

SqliteCommand::SqliteCommand(DataBaseSqlite* db)
{
    m_db   = db;
    m_stmt = NULL;
}

SqliteCommand::SqliteCommand(DataBaseSqlite* db, const string& sql)
{
    m_db   = db;
    m_stmt = NULL;
    Prepare(sql);
}

SqliteCommand::SqliteCommand(DataBaseSqlite* db, const char* sql)
{
    m_db   = db;
    m_stmt = NULL;
    Prepare(sql);
}

SqliteCommand::~SqliteCommand()
{
    sqlite3_finalize(m_stmt);
}

DB_Error SqliteCommand::Prepare(const char* sql)
{
    DB_Error ret = DB_INVAL;
    if (sql && strlen(sql) != 0 && m_stmt == NULL)
    {
        ret = sqlite3_prepare_v2(m_db->m_pDb, sql, -1, &m_stmt, NULL);
    }
    PDEBUG ("ret = %d, db: %p, m_stmt: %p, cmd: %s\n", ret, m_db->m_pDb, m_stmt, sql);
    return ret;
}

DB_Error SqliteCommand::Prepare(const string& sql)
{
    return Prepare(sql.c_str());
}

DB_Error SqliteCommand::Finalize()
{
    return DB_OK;
}

int SqliteCommand::FindParameterIndex(const string& name)
{
    return FindParameterIndex(name.c_str());
}

int SqliteCommand::FindParameterIndex(const char* name)
{
    PDEBUG ("Enter, m_stmt: %p\n", m_stmt);
    int index = 0;
    if (m_stmt && name && strlen(name) != 0)
    {
        index = sqlite3_bind_parameter_index(m_stmt, name);
        PDEBUG ("Finding index for: %s, result: %d\n", name, index);
    }
    return index;
}

DB_Error SqliteCommand::Bind(const char* name, int data)
{
    DB_Error ret = DB_INVAL;

    int index = FindParameterIndex(name);
    if (index)
    {
        ret = sqlite3_bind_int(m_stmt, index, data);
    }
    return ret;
}

DB_Error SqliteCommand::Bind(const char* name, int64 data)
{
    DB_Error ret = DB_INVAL;

    int index = FindParameterIndex(name);
    if (index)
    {
        ret = sqlite3_bind_int64(m_stmt, index, data);
    }
    return ret;
}

DB_Error SqliteCommand::Bind(const char* name, const void* data, int dataLength)
{
    DB_Error ret = DB_INVAL;

    int index = FindParameterIndex(name);
    if (index)
    {
        ret = sqlite3_bind_blob(m_stmt, index, data, dataLength, SQLITE_TRANSIENT);
    }
    return ret;
}

DB_Error SqliteCommand::Bind(const char* name, double data)
{
    DB_Error ret = DB_INVAL;

    int index = FindParameterIndex(name);
    if (index)
    {
        ret = sqlite3_bind_double(m_stmt, index, data);
    }
    return ret;
}

DB_Error SqliteCommand::Bind(const char* name, const char* data)
{
    DB_Error ret = DB_INVAL;

    int index = FindParameterIndex(name);
    PDEBUG ("index: %d\n", index);
    if (index)
    {
        ret = sqlite3_bind_text(m_stmt, index, data, strlen(data), SQLITE_TRANSIENT);
    }
    return ret;
}

DB_Error SqliteCommand::Bind(const string& name, int data)
{
    return Bind(name.c_str(), data);
}

DB_Error SqliteCommand::Bind(const string& name, const void* data, int dataLength)
{
    return Bind(name.c_str(), data, dataLength);
}

DB_Error SqliteCommand::Bind(const string& name, double data)
{
    return Bind(name.c_str(), data);
}

DB_Error SqliteCommand::Bind(const string& name, const char* data)
{
    return Bind(name.c_str(), data);
}

DB_Error SqliteCommand::Execute()
{
    DB_Error  ret = DB_OK;
    PDEBUG ("Executing ... \n");
    while ((ret = sqlite3_step(m_stmt)) == SQLITE_ROW)
    {
        // XXX: Here goes functions to parse results.
    }

    if (ret == SQLITE_DONE)
    {
        ret  = DB_OK;
    }
    return ret;
}

DB_Error DataBaseSqlite::AddUser(shared_ptr<User> user)
{
    DB_Error ret = CreateTable("User");
    if (ret == DB_OK)
    {
        EntryList entries = m_pTableComponent->GetTableFormat("User");
        PDEBUG ("entry size: %lu\n", entries.size());
        if (!entries.empty())
        {
            string sql(INSERT_TABLE);
            sql += string("User") + VALUE + LPARENT;
            EntryList::iterator iter = entries.begin();
            EntryList::iterator end  = entries.end();
            for (; iter != end;)
            {
                sql += "@" + iter->name;
                if (++iter != end)
                {
                    sql += ", ";
                }
            }
            sql += string(RPARENT) + SEMI;

            SqliteCommand cmd(this, sql);

            // Ugly hard code!!
            PDEBUG ("Begin binding\n");
            ret = cmd.Bind("@name", user->name());
            ret = ret ? ret : cmd.Bind("@uuid", user->uuid());
            int64 date = 0;
            if (user->has_reg_date())
            {
                ret = user->reg_date();
            }
            ret = ret ? ret : cmd.Bind("@reg_date", date);

            date = 0;

            if (user->has_last_login())
            {
                date = user->last_login();
            }
            ret = ret ? ret : cmd.Bind("@last_login", date);
            // Bind others ...
            ret = ret ? ret : cmd.Execute();
            // Execute ....
        }
    }
    return ret;
}

DB_Error SqliteCommand::Bind(const char* name, const string& data)
{
    DB_Error ret = DB_INVAL;

    int index = FindParameterIndex(name);
    PDEBUG ("Index: %d\n", index);
    if (index)
    {
        ret = sqlite3_bind_text(m_stmt, index, data.c_str(), data.size(), SQLITE_TRANSIENT);
        PDEBUG ("ret = %d\n", ret);
    }
    return ret;
}

DB_Error SqliteCommand::Bind(const string& name, const string& data)
{
    return Bind(name.c_str(), data);
}


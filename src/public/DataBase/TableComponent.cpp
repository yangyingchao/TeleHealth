#include "TableComponent.h"
#include <LogUtils.h>

typedef struct _Table
{
    const char*  name;
    EntryStruct* entries;
} Table;

//            User Table
// | name   | uuid   | reg_data | last_login |   |
// |--------+--------+----------+------------+---|
// | string | string | uint64   | uint64     |   |

static EntryStruct g_UserTableEntries[] =
{
    { "name",       ET_String, false },
    { "uuid",       ET_String, true },
    { "reg_date",   ET_Uin64,  false  },
    { "last_login", ET_Uin64,  false  },
    {  "",          ET_Invalid,false}
};

static Table g_Tables[] =
{
    { "User", g_UserTableEntries },
    { NULL,   NULL}
};

// XXX:

// name: <create, insert>
typedef struct _TableEntry
{
    char* tableName;
    char* createCmd;
    char* insertCmd;
} TableEntry;


static TableComponent* g_TableComponent = NULL;

TableComponent::TableComponent()
{
}

TableComponent::~TableComponent()
{
}

TableComponent*  TableComponent::GetInstance()
{
    if (g_TableComponent == NULL)
    {
        g_TableComponent = new TableComponent;
        g_TableComponent->Initialize();
    }
    return g_TableComponent;
}

EntryList  TableComponent::GetTableFormat(const string& tableName)
{
    EntryList lst;
    map<string, EntryList>::iterator iter = m_commponentTable.find(tableName);
    if (iter != m_commponentTable.end())
    {
        lst = iter->second;
    }
    return lst;
}

EntryList  TableComponent::GetTableFormat(const char* tableName)
{
    return GetTableFormat(string(tableName));
}

/**
 * @name Initialize - Initializes Table components.
 * @return void
 */
void TableComponent::Initialize()
{
    PDEBUG ("Enter\n");
    // This provides a way to configure tables flexilly..

    int size = sizeof(g_Tables) / sizeof(Table);
    for (int i = 0; i < size; ++i)
    {
        const Table& tbl = g_Tables[i];
        PDEBUG ("Name: %s\n", tbl.name);

        if (tbl.name)
        {
            EntryList lst;
            PDEBUG ("total: %lu\n", sizeof(tbl.entries) / sizeof(EntryStruct));
            EntryStruct* ptr = tbl.entries;
            while (ptr && ptr->type != ET_Invalid)
            {
                PDEBUG ("entry name: %s\n", ptr->name.c_str());
                if (ptr->type != ET_Invalid)
                {
                    lst.push_back(*ptr);
                }
                ++ptr;
            }
            if (!lst.empty())
            {
                m_commponentTable.insert(make_pair(tbl.name, lst));
            }
        }
    }

}

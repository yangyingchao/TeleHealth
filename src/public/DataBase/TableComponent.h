#ifndef _GENERICTABLECOMPONENT_H_
#define _GENERICTABLECOMPONENT_H_

#include "DataBase.h"
#include <list>
#include <map>
typedef enum _EntryType
{
    ET_Invalid = 0,
    ET_String,
    ET_Int64,
    ET_Uin64,
    ET_Int,
    ET_Uint,
    ET_Float,
    ET_Double,
} EntryType;


typedef struct _EntryStruct
{
    string    name;
    EntryType type;
    bool      primary;
} EntryStruct;


typedef list<EntryStruct>   EntryList;

class TableComponent
{
public:
    static TableComponent* GetInstance();

    EntryList GetTableFormat(const string& tableName);
    EntryList GetTableFormat(const char* tableName);

private:
    TableComponent();
    virtual ~TableComponent();
    void Initialize();

    map<string, string> m_TableCreateComands;
    map<string, string> m_TableInsertComands;
    map<string, EntryList> m_commponentTable;
    //  TableName,         EntryName,  Format
};


#endif /* _GENERICTABLECOMPONENT_H_ */

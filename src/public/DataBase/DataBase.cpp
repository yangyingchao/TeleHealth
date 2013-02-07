#include "DataBase.h"
#include "DataBaseSqlite.h"

DataBasePtr DataBaseFactory::GetInstance(DataBaseType type)
{
    DataBasePtr db;
    switch (type)
    {
        case DBT_Sqlite:
        {
            // db = DataBasePtr(new DataBaseSqlite);
            break;
        }

        default:
        {
            break;
        }
    }

    return db;
}

DataBase::DataBase()
{
}

DataBase::~DataBase()
{
}


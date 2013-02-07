#include "KVDB.h"
#include "Utils.h"
#include "KVDBImplLevelDB.h"

/* See description in header file. */
KVDB* KVDB::GetInstance(KVDB_Type type, const char* path)
{
    KVDB* db = NULL;
    if (path && strlen(path) && MakeDirectoryEx(GetDirectorName(path)))
    {
        switch (type)
        {
            case KVDB_LevelDb:
            {
                db = NEW LevelDB(path);
                break;
            }

            default:
            {
                break;
            }
        }
    }

    return db;
}

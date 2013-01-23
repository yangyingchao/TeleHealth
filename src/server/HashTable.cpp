#include "HashTable.h"
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

uint32 StringHashFunction(const char* str)
{
    uint32 hash = 0;
    uint32 i    = 0;
    const char*  key  = str;

    for (; i < strlen(str); ++i)
    {
        hash += *(key + i);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % HASH_SIZE;
}

void HashTableDestroy(HashTable* table)
{
    if (table)
    {
        if (table->entries)
        {
            int i = 0;
            for (; i < table->capacity; ++i)
            {
                TableEntry* entry = &table->entries[i];
                if (entry->key)
                {
                    free(entry->key);
                }
                if (entry->val && table->deFunctor)
                {
                    table->deFunctor(entry->val);
                }
            }
            free(table->entries);
        }
        free(table);
    }
}

HashTable* HashTableCreate(uint32 hashSize, HashFunction cFunctor, DestroyFunction dFunctor)
{
    HashTable* table = malloc(sizeof(HashTable));
    if (table)
    {
        memset(table, 0, sizeof(HashTable));

        table->capacity    = hashSize;
        table->entries     = malloc(sizeof(TableEntry) * hashSize);
        table->hashFunctor = cFunctor ? cFunctor : &StringHashFunction;
        table->deFunctor   = dFunctor;

        if (table->entries)
        {
            memset(table->entries, 0, sizeof(TableEntry) * hashSize);
        }
        else
        {
            HashTableDestroy(table);
            table = NULL;
        }
    }
    return table;
}

int InsertEntry(HashTable* table, char* key, void* val)
{
    int ret = 0;
    if (!table || !key || !val )
    {
        return ret;
    }

    PDEBUG ("KEY: %s, val: %p\n", key, val);

    uint32 index = table->hashFunctor(key);
    // Insert entry into the first open slot starting from index.
    uint32 i;
    for (i = index; i < table->capacity; ++i)
    {
        TableEntry* entry = &table->entries[i];
        if (entry->key == NULL)
        {
            ret        = 1;
            entry->key = key;
            entry->val = val;
            break;
        }
    }
    return ret;
}

/*! Looks for the given data based on key.

  @return void*
*/
void* GetEntryFromHashTable(HashTable* table, char* key)
{
    TableEntry* entry = NULL;
    uint32 index = table->hashFunctor(key);
    int i;
    for (i = index; i < table->capacity; ++i)
    {
        entry = &table->entries[i];
        if (entry->key == NULL)
        {
            return NULL;
        }
        if (strcmp(entry->key, key) == 0)
        {
            break;
        }
    }
    if (entry)
    {
        PDEBUG("Key: %s - %s, val: %p\n",
               key, entry->key, entry->val);
    }
    return entry->val;
}

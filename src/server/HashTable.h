#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "typedefs.h"
#include <stdlib.h>
#include <string.h>

typedef void (*DestroyFunction)(void* data);
typedef uint32  (*HashFunction)(const char* key);

// Hash Tables.
typedef struct _TableEntry
{
    char* key;
    void* val;
} TableEntry;

typedef struct _HashTable
{
    int         capacity;
    TableEntry* entries;

    HashFunction    hashFunctor;
    DestroyFunction deFunctor;
} HashTable;


// Functions.
void HashTableDestroy(HashTable* table);

HashTable* HashTableCreate(uint32 size,
                           HashFunction cFunctor=NULL,
                           DestroyFunction dFunctor=NULL);

int InsertEntry(HashTable* table, char* key, void* val);
void* GetEntryFromHashTable(HashTable* table, char* key);

#endif /* _HASHTABLE_H_ */

#ifndef _LIST_H_
#define _LIST_H_

#define CAS(X, Y, Z)       __sync_bool_compare_and_swap(&(X), Y, Z)

#include <stdlib.h>

typedef struct _record
{
    struct _record* next;
    void*           data;
} record;

#endif /* _LIST_H_ */

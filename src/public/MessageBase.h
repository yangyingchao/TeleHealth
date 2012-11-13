#ifndef _MESSAGEBASE_H_
#define _MESSAGEBASE_H_

#include "typedefs.h"

typedef struct _msg_header
{
    uint8  type;
    uint8  reserved[3];
    uint32 data_size;
} msg_header;

#define HEADER_LENGTH       sizeof(msg_header)

#endif /* _MESSAGEBASE_H_ */

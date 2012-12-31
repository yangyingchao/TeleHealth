#ifndef _MESSAGEBASE_H_
#define _MESSAGEBASE_H_

#include "typedefs.h"

const uint8 TMP_FLAG = 1;

typedef struct _msg_header
{
    uint8  type;
    uint8  attr;
    uint8  reserved[2];
    uint8  ahs_legth;
    uint8  msg_length[3];
} msg_header;

#define HEADER_LENGTH       sizeof(msg_header)

#endif /* _MESSAGEBASE_H_ */

#ifndef _MESSAGEBASE_H_
#define _MESSAGEBASE_H_

#include "typedefs.h"
#include "SmartPointer.h"

const uint8 TMP_FLAG = 1;

class MessageHeader;
typedef shared_ptr<MessageHeader> MessageHeaderPtr;

/** This should be treat as raw message, packet headers need to be added when transport. */
class Message
{
public:
    Message();
    virtual ~Message();
    shared_ptr<MessageHeader> m_header;
    void*  m_data;
    uint32 m_dataSize;
};

typedef shared_ptr<Message> MessagePtr;

#endif /* _MESSAGEBASE_H_ */

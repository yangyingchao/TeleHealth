#ifndef _MESSAGEBASE_H_
#define _MESSAGEBASE_H_

#include "typedefs.h"
#include "SmartPointer.h"

const uint8 TMP_FLAG = 1;

#define HEADER_LENGTH       48

class THMessageHeader;
typedef shared_ptr<THMessageHeader> THMessageHeaderPtr;
typedef shared_ptr<google::protobuf::Message> MessagePtr;

/** This should be treat as raw message, packet headers need to be added when transport. */
class THMessage
{
public:
    THMessage();
    virtual ~THMessage();
    bool LoadHeaderFromBlob(const DataBlobPtr& blob);
    void SetBodyDataBlob(DataBlobPtr blob);
    void SetMessageHeader(THMessageHeaderPtr header);

private:
    THMessageHeaderPtr m_pHeader;      // Here we know how this header looks like.
    MessagePtr         m_pBodyMessage; // But we don't know the real Message.
    DataBlobPtr        m_pBodyBlob;    // So we put serialized data into this blob.
};

typedef shared_ptr<THMessage> THMessagePtr;

#endif /* _MESSAGEBASE_H_ */

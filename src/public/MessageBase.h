#ifndef _MESSAGEBASE_H_
#define _MESSAGEBASE_H_

#include "typedefs.h"
#include "SmartPointer.h"
#include "THMessage.pb.h"

const uint8 TMP_FLAG = 1;

#define HEADER_LENGTH       48

typedef shared_ptr<MessageHeader> MessageHeaderPtr;
typedef shared_ptr<google::protobuf::Message> MessagePtr;

class DataBlob;
typedef shared_ptr<DataBlob> DataBlobPtr;

/** This should be treat as raw message, packet headers need to be added when transport. */
class THMessage
{
public:
    THMessage();
    virtual ~THMessage();

    void SetMessageHeader(const MessageHeaderPtr& header);
    void SetMessageBody(const MessagePtr& msg);
    const MessageHeaderPtr GetMessageHeader() const;

    /*! Load header from datablob.

      This will also prepare DataBlob for message body as a side effect!

      @return true if succeeded, or false other wise.
    */
    bool LoadHeaderFromBlob(const DataBlobPtr& blob);

    const DataBlobPtr GetHeaderBlob();
    const DataBlobPtr GetBodyBlob();

private:
    MessageHeaderPtr m_pHeader;         // Here we know how this header looks like.
    MessagePtr       m_pBodyMessage;    // But we don't know the real Message.
    DataBlobPtr      m_pBodyBlob;       // So we put serialized data into this blob.
    DataBlobPtr      m_pHeaderBlob;
};

typedef shared_ptr<THMessage> THMessagePtr;

#endif /* _MESSAGEBASE_H_ */

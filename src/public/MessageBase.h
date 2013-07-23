#ifndef _MESSAGEBASE_H_
#define _MESSAGEBASE_H_

#include "typedefs.h"
#include "SmartPointer.h"
#include "THMessage.pb.h"
#include "DataBlob.h"

typedef shared_ptr<THMessage> THMessagePtr;

// const uint8 TMP_FLAG = 1;

// #define HEADER_LENGTH       48

// typedef shared_ptr<MessageHeader> MessageHeaderPtr;
// typedef shared_ptr<google::protobuf::Message> MessagePtr;


// typedef shared_ptr<DataBlob> DataBlobPtr;

// extern const int SIZEOFINT16;

// /** This should be treat as raw message, packet headers need to be added when transport. */
// class THMessage
// {
// public:
//     THMessage();
//     virtual ~THMessage();

//     void SetMessageHeader(const MessageHeaderPtr& header);
//     void SetMessageBody(const MessagePtr& msg);
//     const MessageHeaderPtr GetMessageHeader() const;

//     /*! Load header from datablob.

//       This will also prepare DataBlob for message body as a side effect!

//       @return true if succeeded, or false other wise.
//     */
//     bool LoadHeaderFromBlob(const DataBlobPtr& blob);

//     const DataBlobPtr GetHeaderBlob();
//     const DataBlobPtr GetBodyBlob();

//     // Check if this message is valid.
//     bool IsValid();

// private:

//     void UpdateHeaderSize();

//     MessageHeaderPtr m_pHeader;         // Here we know how this header looks like.
//     MessagePtr       m_pBodyMessage;    // But we don't know the real Message.
//     DataBlobPtr      m_pBodyBlob;       // So we put serialized data into m_pBodyBlob, and
//     DataBlobPtr      m_pHeaderBlob;     // first 2 bytes are size of this header.
// };


// typedef shared_ptr<THMessage> THMessagePtr;

// #define PACK_SIZE(X)       (X + sizeof(uint16))


#endif /* _MESSAGEBASE_H_ */

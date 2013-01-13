#include "MessageBase.h"

#include "THMessageBase.h"
#include "THTHMessage.pb.h"

/**
 * @name MessageToBlob - Serialize message into a blob.
 * @param msg -  message to be serialized
 * @param destSize - Number of dest Size, should be greater than ByteSize of
 *                   msg if it is  not -1.
 * @return DataBlobPtr - DataBlob contains message.
 */
DataBlobPtr MessageToBlob(const MessagePtr& msg, int destSize)
{
    DataBlobPtr blob;
    int size = 0;
    if (msg && ((size = msg->ByteSize()) > 0))
    {
        if (destSize != -1)
        {
            if (size <= destSize)
            {
                size = destSize;
            }
            else
            {
                // Packet is too large!!
                goto err;
            }
        }

        blob = DataBlob::GetInstance();
        if (!blob || !blob->PrepareSpace(size) ||
            !msg->SerializeToArray(blob->GetData(), size))
        {
            blob.reset();
        }
    }
err:
    return blob;
}

/* See description in header file. */
THMessage::THMessage()
{
}

/* See description in header file. */
THMessage::~THMessage()
{
    m_pHeader.reset();
    if (m_data)
    {
        delete m_data;
    }
}

/* See description in header file. */
void THMessage::SetMessageHeader(const MessageHeaderPtr& header)
{
    m_pHeader = header;
}

/* See description in header file. */
void THMessage::SetMessageBody(const MessagePtr& msg)
{
    m_pBodyMessage = msg;
}

/* See description in header file. */
bool THMessage::LoadHeaderFromBlob(const DataBlobPtr& blob)
{
    bool ret = false;
    m_pHeader = MessageHeaderPtr(new MessageHeader);
    if (m_pHeader && blob)
    {
        m_pHeaderBlob = blob;
        ret = m_pHeader->ParseFromArray(blob->GetData());
    }
    return ret;
}

/* See description in header file. */
DataBlobPtr THMessage::GetHeaderBlob()
{
    m_pHeaderBlob = MessageToBlob(m_pHeader, HEADER_LENGTH);
    return m_pHeaderBlob;
}

/* See description in header file. */
DataBlobPtr THMessage::GetBodyBlob()
{
    m_pBodyBlob = MessageToBlob(m_pBodyMessage);
    return  m_pBodyBlob;
}


#include "MessageBase.h"
#include "DataBlob.h"


/**
 * @name MessageToBlob - Serialize message into a blob.
 * @param msg -  message to be serialized
 * @param destSize - Number of dest Size, should be greater than ByteSize of
 *                   msg if it is  not -1.
 * @return DataBlobPtr - DataBlob contains message.
 */
DataBlobPtr MessageToBlob(const MessagePtr& msg, int destSize=-1)
{
    DataBlobPtr blob;
    int size = 0;
    if (msg)
    {
        size = msg->ByteSize();
        size = size < destSize ? destSize : size;
        if (size > 0)
        {
            blob = DataBlob::GetInstance();
            if (!blob || !blob->PrepareSpace(size) ||
                !msg->SerializeToArray(blob->GetData(), size))
            {
                blob.reset();
            }
        }
    }

    return blob;
}

/* See description in header file. */
THMessage::THMessage()
{
}

/* See description in header file. */
THMessage::~THMessage()
{
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
    bool ret      = false;
    m_pHeaderBlob = blob;
    m_pHeader.reset(new MessageHeader);
    if (m_pHeader && blob &&
        m_pHeader->ParseFromArray(blob->GetData(), blob->GetDataSize())
        && (m_pBodyBlob = DataBlob::GetInstance()) &&
        m_pBodyBlob->PrepareSpace(m_pHeader->length()))
    {
        ret = true;
    }
    return ret;
}

/* See description in header file. */
const DataBlobPtr THMessage::GetHeaderBlob()
{
    if (!m_pHeaderBlob)
    {
        m_pHeaderBlob = MessageToBlob(m_pHeader, HEADER_LENGTH);
    }
    return m_pHeaderBlob;
}

/* See description in header file. */
const DataBlobPtr THMessage::GetBodyBlob()
{
    if (!m_pBodyBlob)
    {
        m_pBodyBlob = MessageToBlob(m_pBodyMessage);
    }
    return  m_pBodyBlob;
}

/* See description in header file. */
const MessageHeaderPtr THMessage::GetMessageHeader() const
{
    return m_pHeader;
}

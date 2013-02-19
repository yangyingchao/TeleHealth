#include "MessageBase.h"
#include "DataBlob.h"
#include "LogUtils.h"

const int SIZEOFINT16 = sizeof(int16);


/**
 * @name MessageToBlob - Serialize message into a blob.
 * @param msg -  message to be serialized
 * @param isHeader - if it is header, then fill it to HEADER_LENGTH bytes, with the first
 *                   two bytes filled with real size of data.

 * @return DataBlobPtr - DataBlob contains message.
 */
DataBlobPtr MessageToBlob(const MessagePtr& msg, bool isHeader = false)
{
    DataBlobPtr blob;
    if (!msg)
    {
        return blob;
    }

    int dataSize  = msg->ByteSize();
    if (dataSize <= 0)
    {
        return blob;
    }

    if (isHeader)
    {
        assert(dataSize < HEADER_LENGTH);
        dataSize = HEADER_LENGTH;
    }

    int16* ptr = NULL;
    blob       = DataBlob::GetInstance();
    if (blob && blob->PrepareSpace(dataSize) && (ptr = (int16*)(blob->GetData())))
    {
        if (isHeader)
        {
            *ptr = (int16)(msg->ByteSize());
            ++ptr;
        }

        if (!msg->SerializeToArray((void*)ptr, dataSize))
        {
            blob.reset();
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
    UpdateHeaderSize();
}

/* See description in header file. */
void THMessage::SetMessageBody(const MessagePtr& msg)
{
    m_pBodyMessage = msg;
    UpdateHeaderSize();
}

/* See description in header file. */
bool THMessage::LoadHeaderFromBlob(const DataBlobPtr& blob)
{
    bool ret      = false;
    m_pHeaderBlob = blob;
    m_pHeader.reset(new MessageHeader);
    PDEBUG ("header: %p, blob: %p, buf: %p, size: %d\n",
            m_pHeader.get(),
            blob.get(), blob->GetData(), blob->GetDataSize());
    if (m_pHeader && blob)
    {
        int    dataSize = 0;
        int16* ptr      = (int16*)(blob->GetData());
        if (ptr)
        {
            dataSize = (int)(*ptr);
            ptr++;
        }

        ret = m_pHeader->ParseFromArray((void*)ptr, dataSize);
        if (ret)
        {
            m_pBodyBlob = DataBlob::GetInstance();
            m_pBodyBlob->PrepareSpace(m_pHeader->length());
        }
        else
        {
            PDEBUG ("ret = %d\n", ret);
        }
    }

    return ret;
}

/* See description in header file. */
const DataBlobPtr THMessage::GetHeaderBlob()
{
    if (!m_pHeaderBlob)
    {
        m_pHeaderBlob = MessageToBlob(m_pHeader, true);
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

/* See description in header file. */
void THMessage::UpdateHeaderSize()
{
    if (m_pHeader && m_pBodyMessage)
    {
        m_pHeader->set_length(m_pBodyMessage->ByteSize() + SIZEOFINT16);
    }
}

/* See description in header file. */
bool THMessage::IsValid()
{
    int dataSize = m_pHeader->length();
    if (dataSize && (!m_pBodyBlob || !m_pBodyBlob->GetDataSize()))
    {
        return  false;
    }
    return true;
}

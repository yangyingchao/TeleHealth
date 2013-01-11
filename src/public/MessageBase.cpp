#include "THMessageBase.h"
#include "THTHMessage.pb.h"

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

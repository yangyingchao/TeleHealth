#include "MessageBase.h"

/* See description in header file. */
Message::Message()
        : m_data(NULL),
          m_dataSize(0)
{
}

/* See description in header file. */
Message::~Message()
{
    m_header.reset();
    if (m_data)
    {
        delete m_data;
    }
}

#include "DataBlob.h"
#include "LogUtils.h"
#include <string.h>
#include <stdlib.h>


/* See description in header file. */
DataBlob::DataBlob()
        : m_data(NULL),
          m_size(0)
{
}

/* See description in header file. */
DataBlob::~DataBlob()
{
    if (m_data)
    {
        free(m_data);
    }
}

/* See description in header file. */
void* DataBlob::GetData()
{
    return m_data;
}

/* See description in header file. */
uint32 DataBlob::GetDataSize()
{
    return m_size;
}

/* See description in header file. */
bool DataBlob::PrepareSpace(uint32 size)
{
    bool ret = false;
    if (size > 0 && !m_data && (m_data = malloc(size))) // This is single blob, imitable size.
    {
        memset(m_data, 0, size);
        m_size = size;
        ret = true;
    }
    return ret;
}

/* See description in header file. */
shared_ptr <DataBlob> DataBlob::GetInstance()
{
    return shared_ptr<DataBlob> (new DataBlob);
}

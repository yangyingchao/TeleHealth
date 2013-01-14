#ifndef _DATABLOB_H_
#define _DATABLOB_H_

#include "typedefs.h"
#include "SmartPointer.h"

class DataBlob
{
public:
    friend class THMessage;
    virtual ~DataBlob();

    void*  GetData();
    uint32 GetDataSize();
    bool   PrepareSpace(uint32 size);

    static shared_ptr<DataBlob> GetInstance();

private:
    DataBlob();

    void*  m_data;
    uint32 m_size;
};

typedef shared_ptr<DataBlob> DataBlobPtr;

#endif /* _DATABLOB_H_ */

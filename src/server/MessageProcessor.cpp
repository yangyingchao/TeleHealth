#include "MessageProcessor.h"

static shared_ptr<ObjectPool<MessageProcessor> > g_pMessageProcessorPool;

static const int PoolChunkSize = 64;

/* See description in header file. */
MessageProcessorPtr MessageProcessor::GetInstance(MessageHeaderPtr header)
{
    if (!g_pMessageProcessorPool)
    {
        g_pMessageProcessorPool = ObjectPool<MessageProcessor>::GetPool(PoolChunkSize);
    }

    return g_pMessageProcessorPool ? g_pMessageProcessorPool->Get() :
            MessageProcessorPtr();
}

/* See description in header file. */
MessageProcessor::~MessageProcessor()
{
    //XXX: Implement this!
}

/* See description in header file. */
MessageProcessor::MessageProcessor()
{
    //XXX: Implement this!
}

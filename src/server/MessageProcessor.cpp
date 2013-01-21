#include "MessageProcessor.h"

static shared_ptr<ObjectPool<MessageProcessor> > g_pMessageProcessorPool;

static const int PoolChunkSize = 64;

HandlerMap MessageProcessor::m_commandHandlers();

static inline bool IsRequestCommand(Command cmd)
{
    return (bool)(cmd % 2);
}

/* See description in header file. */
MessageProcessorPtr MessageProcessor::GetInstance(MessageHeaderPtr header)
{
    if (!g_pMessageProcessorPool)
    {
        g_pMessageProcessorPool = ObjectPool<MessageProcessor>::GetPool(PoolChunkSize);
    }

    return g_pMessageProcessorPool ? g_pMessageProcessorPool->Get() : MessageProcessorPtr();
}

/* See description in header file. */
MessageProcessor::~MessageProcessor()
{
    //XXX: Implement this!
}

/* See description in header file. */
MessageProcessor::MessageProcessor()
{
}

/* See description in header file. */
THMessagePtr MessageProcessor::ProcessMessage(THMessagePtr msg)
{
    THMessagePtr rsp;
    // No need to check msg again herer.

    MessageHeaderPtr header = msg->GetMessageHeader();
    Command cmd = header->type();
    if (!IsRequestCommand(cmd))
    {
        // XXX: We are expecting Requests, but a response is received.
    }

    HanlderMap::iterator iter = MessageProcessor::m_commandHandlers.find(cmd);
    if (iter != MessageProcessor::m_commandHandlers.end() && iter->second)
    {
        rsp = iter->second(msg, NULL);
    }

    return rsp;
}

/* See description in header file. */
bool MessageProcessor::RegisterCommandHandler(Command cmd, CommandHandler handler)
{
    bool result = false;
    if (IsRequestCommand(cmd) && handler)
    {
        result = m_commandHandlers.insert(make_pair((int)cmd, handler)).second;
    }
    return result;
}

#include "MessageProcessor.h"
#include "Pool.h"
#include <LogUtils.h>

static shared_ptr<ObjectPool<MessageProcessor> > g_pMessageProcessorPool;

static const int PoolChunkSize = 64;

HandlerMap MessageProcessor::m_commandHandlers = HandlerMap();

static inline bool IsRequestCommand(Command cmd)
{
    return (bool)(cmd % 2);
}

/* See description in header file. */
MessageProcessorPtr MessageProcessor::GetInstance(MessageHeaderPtr header)
{
    PDEBUG ("enter\n");
    if (!g_pMessageProcessorPool)
    {
        g_pMessageProcessorPool = ObjectPool<MessageProcessor>::GetPool(PoolChunkSize);
    }

    PDEBUG ("Pool: %p\n", g_pMessageProcessorPool.get());
    return g_pMessageProcessorPool ?
            g_pMessageProcessorPool->GetObject() : MessageProcessorPtr();
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
    PDEBUG ("enter\n");
    ErrorCode err = EC_INVALID_CMD;
    THMessagePtr rsp;
    // No need to check msg again herer.

    MessageHeaderPtr header = msg->GetMessageHeader();
    Command cmd = header->cmd();
    if (IsRequestCommand(cmd))
    {
        HandlerMap::iterator iter = MessageProcessor::m_commandHandlers.find(cmd);
        if (iter != MessageProcessor::m_commandHandlers.end() && iter->second)
        {
            rsp = iter->second(msg, NULL);
            err = EC_OK;
        }
        else
        {
            err = EC_UNSUPPORT_CMD;
        }
    }

    if (err)
    {
        rsp = GenericErrorResponse(header, err);
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

/* See description in header file. */
THMessagePtr MessageProcessor::GenericErrorResponse(MessageHeaderPtr header, ErrorCode err)
{
    THMessagePtr rsp(new THMessage);
    MessageHeaderPtr newHeader(new MessageHeader);
    newHeader->set_cmd(header->cmd());
    newHeader->set_version(header->version());
    newHeader->set_session_id(header->session_id());
    newHeader->set_length(0);
    newHeader->set_extra_info(err);
    rsp->SetMessageHeader(newHeader);

    return  rsp;
}

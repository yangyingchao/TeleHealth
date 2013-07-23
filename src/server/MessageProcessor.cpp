#include "MessageProcessor.h"
#include "Pool.h"
#include <LogUtils.h>

HandlerMap MessageProcessor::m_commandHandlers = HandlerMap();

static inline bool IsRequestCommand(Command cmd)
{
    return (bool)(cmd % 2);
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
bool MessageProcessor::RegisterCommandHandler(Command cmd, CommandHandler handler)
{
    bool result = false;
    if (IsRequestCommand(cmd) && handler)
    {
        result = m_commandHandlers.insert(make_pair((int)cmd, handler)).second;
    }
    return result;
}

// Implementation of Private data of MessageProcessor.
/* See description in header file. */
MsgPrsPrivate::MsgPrsPrivate()
{
}

/* See description in header file. */
MsgPrsPrivate::~MsgPrsPrivate()
{
    Reset();
}

/* See description in header file. */
void MsgPrsPrivate::Reset()
{
    m_errMsg.clear();
    m_account.reset();
    m_sessionId.clear();
}

/* See description in header file. */
ZmqMessagePtr MessageProcessor::ProcessMessage(const ZmqMessagePtr& msg)
{
    PDEBUG ("enter\n");
    ErrorCode err = EC_UNEXPECTED;
    THMessage tmsg;
    if (ZmqMsg2Message(msg, tmsg))
    {
        //FIXME
        // return GenericErrorResponse(msg, err);
    }

    Command cmd = tmsg.cmd();
    THMessagePtr rsp;
    if (IsRequestCommand(cmd))
    {
        HandlerMap::iterator iter = MessageProcessor::m_commandHandlers.find(cmd);
        if (iter != MessageProcessor::m_commandHandlers.end() && iter->second)
        {
            rsp = iter->second(tmsg, m_privateData);
            err = EC_OK;
        }
        else
        {
            err = EC_UNSUPPORT_CMD;
        }
    }
    else
    {
        err = EC_INVALID_CMD;
    }

    if (!rsp && err == EC_OK)
    {
        err = EC_UNEXPECTED;
    }
    if (err)
    {
        rsp = GenericErrorResponse(tmsg, err);
    }

    return ZmqMessage::GetInstance(rsp.get());
}

/* See description in header file. */
THMessagePtr MessageProcessor::GenericErrorResponse(const THMessage& tmsg, ErrorCode err)
{
    THMessagePtr rsp;
    // MessageHeaderPtr newHeader(NEW MessageHeader);
    // newHeader->set_cmd(header->cmd());
    // newHeader->set_version(header->version());
    // newHeader->set_session_id(header->session_id());
    // newHeader->set_length(0);
    // newHeader->set_extra_info(err);
    // rsp->SetMessageHeader(newHeader);

    return  rsp;
}

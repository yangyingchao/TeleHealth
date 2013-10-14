#include "MessageProcessor.h"
#include "Pool.h"
#include <LogUtils.h>

typedef map<int, CommandHandler> HandlerMap;
static HandlerMap g_commandHandlers;

// The idea here is not share any writable data to multiple thread.
// The only one shared between each thread is g_commandHandlers which is read-only.

static inline bool IsRequestCommand(Command cmd)
{
    return (bool)(cmd % 2);
}

/* See description in header file. */
bool RegisterCommandHandler(Command cmd, CommandHandler handler)
{
    bool result = false;
    if (IsRequestCommand(cmd) && handler)
    {
        result = g_commandHandlers.insert(make_pair((int)cmd, handler)).second;
    }
    return result;
}


/* See description in header file. */
static inline
THMessagePtr GenericErrorResponse(const THMessage& tmsg, ErrorCode err)
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

/* See description in header file. */
ZmqMessagePtr ProcessMessage(const ZmqMessagePtr& msg,
                             const ZmqContextPtr& gContext,
                             const ConfigParserPtr& gConfig)
{
    PDEBUG ("enter\n");
    ErrorCode err = EC_UNEXPECTED;
    THMessage tmsg;
    if (!ZmqMsg2Message(msg, tmsg))
    {
        //FIXME
        // return GenericErrorResponse(msg, err);
    }

    Command cmd = tmsg.cmd();
    //TODO: Add checks of length version, session_id.
    THMessagePtr rsp;
    if (IsRequestCommand(cmd))
    {
        HandlerMap::iterator iter = g_commandHandlers.find(cmd);
        if (iter != g_commandHandlers.end() && iter->second)
        {
            rsp = iter->second(tmsg, gContext, gConfig);
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

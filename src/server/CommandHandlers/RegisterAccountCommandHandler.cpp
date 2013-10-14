// ZZZ_TARGET_CMD: RegisterUserReq
#ifdef TB_SYM_CHECK
#include <MessageBase.h>
#include <MessageProcessor.h>
#include <ConfigParser.h>
#endif
// Client App should ensure required parts of Account is filled!

THMessagePtr zzz_RegisterAccountCommandHandler(const THMessage& msg,
                                               const ZmqContextPtr& gContext,
                                               const ConfigParserPtr& gConfig)
{
    printf("Called!\n");
    // TODO: Remove this ifdef!
#if 0
    string&  errMsg(privData.m_errMsg);
    AM_Error err = EC_OK;
    MessageHeaderPtr header = msg->GetMessageHeader();
    do
    {
        err = EC_INVALID_ARG;
        goto out;
    }

    if (!priv.m_dbSock)
    {
        priv.m_dbSock.reset(NEW ZmqSocket(priv.m_pContext, ZMQ_REQ));
        if (!priv.m_dbSock)
        {
            err = EC_NOMEM;
            goto out;
        }

        if (priv.m_dbSock->Connect(priv.m_pConfig->GetDBAddress()))
        {
            err = EC_SRV_INTERNAL;
            goto out;
        }
    }

    zmsg = ZmqMessage::GetInstance(&msg);
    ret = priv.m_dbSock->Send(zmsg);
    if (ret)
    {
        // Forward request to db thread.
        zmsg = priv.m_dbSock->Recv();
    }
out:
    rsp.reset(NEW THMessage);
    if (zmsg && rsp && ZmqMsg2Message<THMessage>(zmsg, *rsp))
    {
        return rsp;
    }

    // Reuse and update header.
    header->set_cmd(RegisterUserRsp);
    header->set_length(0);
    header->set_session_id(privData.m_sessionId);
    header->set_extra_info((uint32)err); // Use this extra_info as error code.

    AccountRegisterResponse* body = NEW AccountRegisterResponse;

    if (!err && body)
    {
        body->set_err(err);
        // Generate a ServiceLists and set it as body of message.
        ServiceLists* srvList = body->mutable_srv_list();
        if (srvList)
        {
            // Add services provided to this account.
            srvList->add_types(VST_PhysicalInfo);
            srvList->set_tips("Welcome, you!");
        }
        msg->SetMessageBody(MessagePtr(body));
    }
#endif // End of #if 0

    return  THMessagePtr();
}

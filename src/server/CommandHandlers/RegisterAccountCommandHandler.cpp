// ZZZ_TARGET_CMD: RegisterUserReq
#ifdef TB_SYM_CHECK
#include <MessageBase.h>
#include <MessageProcessor.h>
#include <ConfigParser.h>
#endif
// Client App should ensure required parts of Account is filled!

THMessagePtr zzz_RegisterAccountCommandHandler(const THMessage& msg,
                                               MsgPrivate& priv)
{
    printf("Called!\n");

    string        errMsg;
    ZmqMessagePtr zmsg;
    THMessagePtr  rsp;
    ErrorCode     err  = EC_OK;
    const char*   data = NULL;
    int           ret  = 0;

    if (!msg.has_data() || !(data = msg.data().c_str()))
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
    rsp->set_cmd(RegisterUserRsp);
    rsp->set_session_id("");
    // version should be set outside.
    AccountRegisterResponse body;
    body.set_err(err);
    string dataOut;
    (void) body.SerializeToString(&dataOut);
    rsp->set_data(dataOut);
    rsp->set_length(body.ByteSize());
    return  rsp;
}

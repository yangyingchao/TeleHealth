// ZZZ_TARGET_CMD: RegisterUserReq
#ifdef TB_SYM_CHECK
#include <MessageBase.h>
#include <MessageProcessor.h>
#endif
// Client App should ensure required parts of Account is filled!

THMessagePtr zzz_RegisterAccountCommandHandler(const THMessage& msg, MsgPrsPrivate& privData)
{
    printf("Called!\n");

    string&  errMsg(privData.m_errMsg);
    AM_Error err = EC_OK;
    do
    {
        const char* data = NULL;
        if (!msg.has_data() || !(data = msg.data().c_str()))
        {
            err = EC_INVALID_ARG;
            break;
        }

        // Parse it into a Account class.
        Account*    account = new Account;
        if (!account->ParseFromArray(data, msg.data().size()))
        {
            errMsg = "Failed to parse message.";
            err = EC_INVALID_ARG;
            break;
        }

        AM_Error err = RegisterAccount(account);
        if (err)
        {
            break;
        }

        // Update private data.
        privData.m_status = account->status();
        privData.m_sessionId = "FAKE_SEESION_ID"; // XXX:

    } while (0);

    THMessagePtr rsp(NEW THMessage);
    // Reuse and update header.
    rsp->set_cmd(RegisterUserRsp);
    rsp->set_length(0);
    rsp->set_session_id(privData.m_sessionId);

    AccountRegisterResponse body;
    body.set_err(err);
    if (!err)
    {
        // Generate a ServiceLists and set it as body of message.
        ServiceLists* srvList = body.mutable_srv_list();
        if (srvList)
        {
            // Add services provided to this account.
            srvList->add_types(VST_PhysicalInfo);
            srvList->set_tips("Welcome, you!");
        }
    }

    string dataOut;
    (void) body.SerializeToString(&dataOut);
    rsp->set_data(dataOut);
    return  rsp;
}

// ZZZ_TARGET_CMD: RegisterUserReq


// Client App should ensure required parts of Account is filled!

THMessagePtr zzz_RegisterAccountCommandHandler(THMessagePtr msg, MsgPrsPrivate& privData)
{
    printf("Called!\n");

    string&  errMsg(privData.m_errMsg);
    AM_Error err = EC_OK;
    MessageHeaderPtr header = msg->GetMessageHeader();
    do
    {

        if (privData.m_account)                           // private data is not empty
        {
            errMsg = "Data corrupt!";
            err = EC_INVALID_ARG;
            break;
        }

        // Parse it into a Account class.
        DataBlobPtr blob    = msg->GetBodyBlob();
        Account*    account = new Account;
        privData.m_account.reset(account);
        if (!account->ParseFromArray(blob->GetData(), blob->GetDataSize()))
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
    return  msg;
}

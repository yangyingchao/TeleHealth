// ZZZ_TARGET_CMD: RegisterUserReq


// Client App should ensure required parts of Account is filled!

THMessagePtr zzz_RegisterAccountCommandHandler(THMessagePtr msg, MsgPrsPrivate& privData)
{
    printf("Called!\n");

    string&  errMsg(privData.m_errMsg);
    do
    {
        DataBlobPtr    blob;
        unsigned char* data     = NULL;
        uint16         dataSize = 0;

        // Check if this message is valid.
        if (privData.m_account ||                           // private data is not empty
            !(blob = msg->GetBodyBlob()) ||                 // No blob
            !(data = (unsigned char*)(blob->GetData())) ||  // No Data
            !blob->GetDataSize() ||                         // size of Blob is zero
            !(dataSize = *((uint16*)data)))                 // size of real data is zero
        {
            errMsg = "Data corrupt!";
            break;
        }

        data += SIZEOFINT16;

        // Parse it into a Account class.
        Account* account = new Account;
        privData.m_account.reset(account);
        if (!account->ParseFromArray(data, dataSize))
        {
            errMsg = "Failed to parse message.";
            break;
        }

        AM_Error err = RegisterAccount(account);
        if (err)
        {
            const char* msg = AM_ErrorStringify(err);
            if (msg)
            {
                errMsg = msg;
            }
            break;
        }

        // Update private data.
        privData.m_status = account->status();
        privData.m_sessionId = "FAKE_SEESION_ID"; // XXX:

        // Reuse and update header.
        MessageHeaderPtr header = msg->GetMessageHeader();
        header->set_cmd(RegisterUserRsp);
        header->set_length(0);
        header->set_session_id(privData.m_sessionId);

        // Generate a ServiceLists and set it as body of message.
        ServiceLists* body = NEW ServiceLists;
        if (!body)
        {
            errMsg = "No mem";
            break;
        }

        // Add services provided to this account.
        body->add_types(VST_PhysicalInfo);
        body->set_tips("Welcome, you!");
        msg->SetMessageBody(MessagePtr(body));

    } while (0);

    if (!errMsg.empty())
    {
        // XXX: Handle error here and generate reply.
    }

    return  msg;
}

// ZZZ_TARGET_CMD: RegisterUserReq


// Client App should ensure required parts of Account is filled!

THMessagePtr zzz_RegisterAccountCommandHandler(THMessagePtr msg, MsgPrsPrivate& privData)
{
    printf("Called!\n");

    THMessagePtr rsp;
    string&  errMsg(privData.m_errMsg);
    do
    {
        DataBlobPtr    blob;
        unsigned char* data     = NULL;
        uint16         dataSize = 0;

        // Check if this message is valid.
        if (privData.m_account ||                                     // private data is not empty
            !msg ||                                         // No msg
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

        // TODO: Succeeded in registering, generate a response and return.

    } while (0);

    return  rsp;
}

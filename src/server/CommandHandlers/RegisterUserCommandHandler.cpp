// ZZZ_TARGET_CMD: RegisterUserReq

THMessagePtr zzz_RegisterUserCommandHandler(THMessagePtr msg, void* privData)
{
    THMessagePtr rsp(msg);
    printf("Called!\n");
    return  rsp;
}

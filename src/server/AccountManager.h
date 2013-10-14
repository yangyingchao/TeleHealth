#ifndef _ACCOUNTMANAGER_H_
#define _ACCOUNTMANAGER_H_
// TODO: Remove this ifdef!
#if 0

#include "MessageBase.h"


class Account;
// For now, just implement some functions that can be used by all threads safely.

// Account Management Errors.


typedef ErrorCode   AM_Error;
class MsgPrivate;

AM_Error RegisterAccount(Account* account, MsgPrivate& priv);

AM_Error AccountLogoff(Account* account, MsgPrivate& priv);
#endif // End of #if 0

#endif /* _ACCOUNTMANAGER_H_ */

#ifndef _ACCOUNTMANAGER_H_
#define _ACCOUNTMANAGER_H_

#include "MessageBase.h"


class Account;
// For now, just implement some functions that can be used by all threads safely.

// Account Management Errors.


typedef ErrorCode   AM_Error;

bool InitializeAccountManager(const char* path);

void DestroyAccountManager();

AM_Error RegisterAccount(Account* account);

AM_Error AccountLogoff(Account* account);
#endif /* _ACCOUNTMANAGER_H_ */

#ifndef _ACCOUNTMANAGER_H_
#define _ACCOUNTMANAGER_H_

class Account;
// For now, just implement some functions that can be used by all threads safely.

// Account Management Errors.
typedef enum _AM_Error
{
    AE_OK = 0,
    AE_INVALID,
    AE_EXISTED,
    AE_MAX,
} AM_Error;

const char* AM_ErrorStringify(AM_Error errCode);

bool InilializeAccountManager(const char* path);

AM_Error RegisterAccount(Account* account);

#endif /* _ACCOUNTMANAGER_H_ */

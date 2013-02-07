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
    AE_NOMEM,
    AE_BUSY,
    AE_DB,
    AE_MAX,
} AM_Error;

const char* AM_ErrorStringify(AM_Error errCode);

bool InitializeAccountManager(const char* path);

void DestroyAccountManager();

AM_Error RegisterAccount(Account* account);

AM_Error AccountLogoff(Account* account);
#endif /* _ACCOUNTMANAGER_H_ */

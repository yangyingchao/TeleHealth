#ifndef _ACCOUNTMANAGER_H_
#define _ACCOUNTMANAGER_H_

class Account;
// For now, just implement some functions that can be used by all threads safely.

bool InilializeAccountManager(const char* path);

bool IsAccountExsited(Account* account);

bool RegisterAccount(Account* account);

bool IsAccountValid(Account* account);

#endif /* _ACCOUNTMANAGER_H_ */

#ifndef _ACCOUNTMANAGER_H_
#define _ACCOUNTMANAGER_H_

// For now, just implement some functions that can be used by all threads safely.

bool InilializeAccountManager(const char* path);

bool IsAccountExsited();

bool IsAccountValid();

#endif /* _ACCOUNTMANAGER_H_ */

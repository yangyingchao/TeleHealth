#ifndef _UNITS_H_
#define _UNITS_H_

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long int int64;
typedef unsigned long long int uint64;
#endif

#endif /* _UNITS_H_ */

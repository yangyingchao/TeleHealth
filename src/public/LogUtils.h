#ifndef _LOGUTILS_H_
#define _LOGUTILS_H_
#include <stdio.h>

#if !defined (PDEBUG)
#ifdef _WIN32
#define PDEBUG(fmt, ...)                                                \
    do {                                                                \
    const char* file = __FILE__, *ptr = strstr(file, "..");             \
    if (!ptr) ptr = file; else while (*ptr == '.' || *ptr == '\\' || *ptr == '/') ++ptr; \
        printf("%s(%d)-%s:\t: ", ptr, __LINE__,__FUNCTION__);           \
        printf(fmt, __VA_ARGS__);                                       \
    } while(0)
#else
#define PDEBUG(fmt, args...)                                            \
    do {                                                                \
        const char* file = __FILE__, *ptr = strstr(file, "..");         \
        if (!ptr) ptr = file; else while (*ptr == '.' || *ptr == '\\' || *ptr == '/') ++ptr; \
                                          printf("%s(%d)-%s:\t: ", ptr, __LINE__,__FUNCTION__); \
                                          printf(fmt, ##args);          \
                                          } while(0)
#endif
#endif

#endif /* _LOGUTILS_H_ */

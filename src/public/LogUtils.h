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

#define handle_error(msg)                               \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#ifdef DEBUG
int xStep = 1;
#define OUT_STEP(fmt, args...)                                          \
    do {                                                                \
        const char* file = __FILE__, *ptr = strstr(file, "..");         \
        if (!ptr) ptr = file; else while (*ptr == '.' || *ptr == '\\' || *ptr == '/') ++ptr; \
        printf("%s(%d)\t-%s, Step - %d:\t", ptr, __LINE__,__FUNCTION__, xStep++); \
        printf(fmt, ##args);                                            \
    } while(0)
#else
#define OUT_STEP(fmt, ...)
#endif


#endif /* _LOGUTILS_H_ */

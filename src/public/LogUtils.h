#ifndef _LOGUTILS_H_
#define _LOGUTILS_H_
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#ifdef _WIN32
#define PDEBUG(fmt, ...)                                        \
    do {                                                        \
        printf("%s(%d)-%s:\t",__FILE__,__LINE__,__FUNCTION__);  \
        printf(fmt, __VA_ARGS__);                               \
    } while(0)
#else
#define PDEBUG(fmt, args...)                                    \
    do {                                                        \
        printf("%s(%d)-%s:\t",__FILE__,__LINE__,__FUNCTION__);  \
        printf(fmt, ##args);                                    \
        fflush(stdout);                                         \
} while(0)
#endif
#else
#ifdef _WIN32
#define PDEBUG(fmt, ...) ;
#else
#define PDEBUG(fmt, args...)  ;
#endif
#endif


#endif /* _LOGUTILS_H_ */

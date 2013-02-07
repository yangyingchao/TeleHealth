#ifndef _TESTMACROS_H_
#define _TESTMACROS_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/Test.h>
#include <iostream>

using namespace std;
using namespace CppUnit;


#ifndef CPPUNIT_ASSERT_NOT_NULL
#define CPPUNIT_ASSERT_NOT_NULL(X)       CPPUNIT_ASSERT(X != NULL)
#endif

class VerboseTextProgressListener : public TextTestProgressListener
{
public:
    VerboseTextProgressListener() {}
    virtual ~VerboseTextProgressListener(){}

    void startTest(Test* test)
    {
        CPPUNIT_ASSERT_NOT_NULL(test);
        printf("Running test case: %s.", test->getName().c_str());
    }

    void endTest(Test* test)
    {
        printf("\n");
    }
};


#if !defined (PDEBUG)
#ifdef _WIN32
#define PDEBUG(fmt, ...)                                                \
    do {                                                                \
        const char* file = __FILE__, *ptr = strstr(file, "..");         \
        if (!ptr) ptr = file; else while (*ptr == '.' || *ptr == '\\' || *ptr == '/') ++ptr; \
        printf("%s(%d)-%s\t: ", ptr, __LINE__,__FUNCTION__);            \
        printf(fmt, __VA_ARGS__);                                       \
    } while(0)
#else
#define PDEBUG(fmt, args...)                                            \
    do {                                                                \
        const char* file = __FILE__, *ptr = strstr(file, "..");         \
        if (!ptr) ptr = file; else while (*ptr == '.' || *ptr == '\\' || *ptr == '/') ++ptr; \
        printf("%s(%d)-%s\t: ", ptr, __LINE__,__FUNCTION__);            \
        printf(fmt, ##args);                                            \
    } while(0)
#endif /*End of if _WIN32*/
#endif  /*End of if PDEBUG*/


#endif /* _TESTMACROS_H_ */

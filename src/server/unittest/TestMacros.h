#ifndef _TESTMACROS_H_
#define _TESTMACROS_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <iostream>

using namespace std;

#ifndef CPPUNIT_ASSERT_NOT_NULL
#define CPPUNIT_ASSERT_NOT_NULL(X)       CPPUNIT_ASSERT_EQUAL((void*)(!(X)), (void*)NULL)
#endif

#endif /* _TESTMACROS_H_ */

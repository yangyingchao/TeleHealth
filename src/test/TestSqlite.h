#ifndef _TESTSQLITE_H_
#define _TESTSQLITE_H_

#include "cppunit/extensions/HelperMacros.h"

class TestSqlite : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestSqlite);

    // Add test cases here
    CPPUNIT_TEST(testDBCreate);
    CPPUNIT_TEST(TestUserAdd);

    CPPUNIT_TEST_SUITE_END();
protected:

public:
    TestSqlite() {}

    /**
     * @name setUp - Initializes the test fixture.
     * @return void
     */
    void setUp ();

    /**
     * @name tearDown - clean resources for this fixtrure
     * @return void
     */
    void tearDown();

    void testDBCreate ();
    void TestUserAdd();

};


#endif /* _TESTSQLITE_H_ */

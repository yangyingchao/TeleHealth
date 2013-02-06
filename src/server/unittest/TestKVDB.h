#ifndef _TESTKVDB_H_
#define _TESTKVDB_H_

#include "TestMacros.h"

class KVDB;

class TestKVDB : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestKVDB);

    // Add test cases here
    CPPUNIT_TEST(TestAddEntry);
    CPPUNIT_TEST(TestAddDupilicatedEntry);
    CPPUNIT_TEST(TestDeleteEntry);
    CPPUNIT_TEST(TestDeleteEmptyEntry);
    CPPUNIT_TEST(TestGetEntry);
    CPPUNIT_TEST(TestUpdateEntry);
    CPPUNIT_TEST(TestUpdateNonexistEntry);

    CPPUNIT_TEST_SUITE_END();

public:
    TestKVDB();
    virtual ~TestKVDB();

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


    // Real test cases.
    void TestAddEntry();

    void TestAddDupilicatedEntry();

    void TestDeleteEntry();

    void TestDeleteEmptyEntry();

    void TestGetEntry();

    void TestUpdateEntry();

    void TestUpdateNonexistEntry();

private:
    KVDB* m_db;
};

#endif /* _TESTKVDB_H_ */

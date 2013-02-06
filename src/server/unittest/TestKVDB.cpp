#include "TestKVDB.h"
#include "../KVDBImplLevelDB.cpp"
#include "../KVDB.cpp"

/* See description in header file. */
TestKVDB::TestKVDB()
{
    //XXX: Implement this!
}

/* See description in header file. */
TestKVDB::~TestKVDB()
{
    //XXX: Implement this!
}

/* See description in header file. */
void TestKVDB::setUp()
{
    m_db = KVDB::GetInstance(KVDB::KVDB_LevelDb, "/tmp/test.db");
    CPPUNIT_ASSERT_NOT_NULL(m_db);
}

/* See description in header file. */
void TestKVDB::tearDown()
{
    if (m_db)
    {
        delete m_db;
    }
}

/* See description in header file. */
void TestKVDB::TestAddEntry()
{
    cout << "Variable = " << m_db << endl;
}

/* See description in header file. */
void TestKVDB::TestAddDupilicatedEntry()
{
    cout << "Variable = " << m_db << endl;
}

/* See description in header file. */
void TestKVDB::TestDeleteEntry()
{
    cout << "Variable = " << m_db << endl;
    //XXX: Implement this!
}

/* See description in header file. */
void TestKVDB::TestDeleteEmptyEntry()
{
    cout << "Variable = " << m_db << endl;
    //XXX: Implement this!
}

/* See description in header file. */
void TestKVDB::TestGetEntry()
{
    cout << "Variable = " << m_db << endl;
    //XXX: Implement this!
}

/* See description in header file. */
void TestKVDB::TestUpdateEntry()
{
    cout << "Variable = " << m_db << endl;
    //XXX: Implement this!
}

/* See description in header file. */
void TestKVDB::TestUpdateNonexistEntry()
{
    cout << "Variable = " << m_db << endl;
    //XXX: Implement this!
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestKVDB, "TestKVDB" );

#include "TestKVDB.h"
#include "../KVDBImplLevelDB.cpp"
#include "../KVDB.cpp"
#include <unistd.h>
#include <stdlib.h>

static const string TEST_KEY("key");
static const string TEST_KEY2("key2");
static const string TEST_VAL("val");
static const string TEST_VAL2("val2");
static const string EMPTY_STRING;

/* See description in header file. */
TestKVDB::TestKVDB()
        : m_db(NULL)
{
}

/* See description in header file. */
TestKVDB::~TestKVDB()
{
}

/* See description in header file. */
void TestKVDB::setUp()
{
    int ret = system("rm -rf /tmp/test.db");

    CPPUNIT_ASSERT_EQUAL(ret, 0);

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
    bool result = m_db->AddKVPair(TEST_KEY, TEST_VAL);
    CPPUNIT_ASSERT(result);
}

/* See description in header file. */
void TestKVDB::TestAddDupilicatedEntry()
{
    CPPUNIT_ASSERT(m_db->AddKVPair(TEST_KEY, TEST_VAL));
    CPPUNIT_ASSERT(!m_db->AddKVPair(TEST_KEY, TEST_VAL));
}

/* See description in header file. */
void TestKVDB::TestDeleteEntry()
{
    CPPUNIT_ASSERT(m_db->AddKVPair(TEST_KEY, TEST_VAL));
    CPPUNIT_ASSERT(m_db->DeleteKVPair(TEST_KEY));
    CPPUNIT_ASSERT_EQUAL(m_db->DeleteKVPair(EMPTY_STRING), false);
}

/* See description in header file. */
void TestKVDB::TestGetEntry()
{
    CPPUNIT_ASSERT(m_db->AddKVPair(TEST_KEY, TEST_VAL));
    string val;
    CPPUNIT_ASSERT(m_db->GetValue(TEST_KEY, val));
    CPPUNIT_ASSERT(!val.empty() && val == TEST_VAL);
}

/* See description in header file. */
void TestKVDB::TestUpdateEntry()
{
    CPPUNIT_ASSERT(m_db->AddKVPair(TEST_KEY, TEST_VAL));
    CPPUNIT_ASSERT(m_db->UpdateValue(TEST_KEY, TEST_VAL2));
}

/* See description in header file. */
void TestKVDB::TestUpdateNonexistEntry()
{
    CPPUNIT_ASSERT(!m_db->UpdateValue(TEST_KEY2, TEST_VAL2));
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestKVDB);

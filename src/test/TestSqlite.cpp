#include "TestSqlite.h"

#include "TestSqlite.h"
#include <DataBase.h>
#include <DataBaseSqlite.h>
#include <Utils.h>
#include <DBTables.pb.h>

using namespace std;

void TestSqlite::setUp()
{
}

void TestSqlite::tearDown()
{
}

void TestSqlite::testDBCreate()
{
    cout << "... Testing " << __FUNCTION__ << " ..."<< endl;

    DataBasePtr db = DataBaseFactory::GetInstance(DBT_Sqlite);
    CPPUNIT_ASSERT(db);

    DB_Error ret = db->Open("/tmp/test.db");
    CPPUNIT_ASSERT_EQUAL(DB_OK, ret);

    DataBaseSqlite* sqlDB = static_cast<DataBaseSqlite*>(db.get());
    CPPUNIT_ASSERT(sqlDB);

    ret = db->CreateTable("User");
    cout << "ret = " << (int)ret << endl;

    CPPUNIT_ASSERT_EQUAL(DB_OK, ret);

    cout << "ret = " << (int)ret << endl;

}

void TestSqlite::TestUserAdd()
{
    cout << "... Testing " << __FUNCTION__ << " ..."<< endl;

    DataBasePtr db = DataBaseFactory::GetInstance(DBT_Sqlite);
    CPPUNIT_ASSERT(db);

    DB_Error ret = db->Open("/tmp/test.db");
    CPPUNIT_ASSERT_EQUAL(DB_OK, ret);

    shared_ptr<User> user(new User);
    user->set_name("yyc");
    string uuid = GenerateUUID();
    user->set_uuid(uuid);
    ret = db->AddUser(user);
    CPPUNIT_ASSERT_EQUAL(DB_OK, ret);
    cout << "ret = " << ret << endl;

    // Should fail if add it again.
    ret = db->AddUser(user);
    if (ret == DB_OK)
    {
        CPPUNIT_FAIL("Should not success.");
    }
    cout << "ret = " << ret << endl;
}


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TestSqlite, "alltest" );


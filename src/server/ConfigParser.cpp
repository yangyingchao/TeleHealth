#include "ConfigParser.h"
#include <Utils.h>

static ConfigParserPtr gConfigParser;

static const char* FAKE_PORT = "5678";

/* See description in header file. */
ConfigParser::ConfigParser()
        : m_root("/var/tmp/server"),
          m_valid(true),
          m_externalAddr("tcp://*5555"),
          m_dealerAddr("tcp://*:5556"),
          m_DBAddress("ipc://THDatabase.ipc")
{
    if (!dir_exist(m_root.c_str()) && !MakeDirectory(m_root.c_str()))
    {
        m_valid = false;
    }
}

/* See description in header file. */
ConfigParser::~ConfigParser()
{
    //XXX: Implement this!
}

/* See description in header file. */
ConfigParserPtr ConfigParser::GetConfigParserWithParams(int argc, char** argv)
{
    //XXX: Implement this!
    if (!gConfigParser)
    {
        gConfigParser.reset( NEW ConfigParser());
    }
    if (gConfigParser && !gConfigParser->m_valid)
    {
        gConfigParser.reset();
    }

    return gConfigParser;
}

/* See description in header file. */
const string& ConfigParser::GetRootDirectory() const
{
    return m_root;
}

/* See description in header file. */
const uint32 ConfigParser::GetIoThreadNumber() const
{
    return 1;
}

/* See description in header file. */
const string& ConfigParser::GetExternalAddress() const
{
    return m_externalAddr;
}

/* See description in header file. */
const string& ConfigParser::GetDBAddress() const
{
    return m_dbAddr;
}

/* See description in header file. */
const string& ConfigParser::GetDealerAddress() const
{
    return m_dealerAddr;
}

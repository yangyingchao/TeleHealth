#include "ConfigParser.h"
#include <Utils.h>

static ConfigParserPtr gConfigParser;

/* See description in header file. */
ConfigParser::ConfigParser()
        : m_root("/var/tmp/server"),
          m_valid(true)
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
 ConfigParser::GetConfigParserWithParams(int argc, char** argv)
{
    //XXX: Implement this!
    if (!gConfigParser)
    {
        gConfigParser = new ConfigParser();
    }
    if (gConfigParser && !gConfigParser->m_valid)
    {
        gConfigParser.reset();
    }

    return gConfigParser;
}

/* See description in header file. */
const string& ConfigParser::GetRootDirectory()
{
    return m_root;
}

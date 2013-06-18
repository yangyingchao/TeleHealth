#include "ConfigParser.h"
#include <Utils.h>
#include <getopt.h>
#include "LogUtils.h"

static const char* FAKE_PORT = "5678";

/* See description in header file. */
ConfigParser::ConfigParser(const string& configFile)
        : m_root("/var/tmp/server"),
          m_valid(true),
          m_externalAddr("tcp://localhost:5555"),
          m_dealerAddr("tcp://localhost:5556"), // Should set to address that not exposed.
          m_dbAddr("ipc://THDatabase.ipc")
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
shared_ptr <ConfigParser>  ConfigParser::GetConfigParserWithParams(int argc,
                                                                   char** argv)
{
    struct option options[] = {
#define X(x, y, z)       {#x, y, 0, z}
        X(config,  required_argument, 'c'),
        X(serverAddr, required_argument, 's'),
        X(dbAddr, required_argument, 'd'),
        X(workerAddr, required_argument, 'w'),
        {NULL, 0, 0, 0}
    };

    int c           = 0;
    int optionIndex = 0;

    string serverAddr("tcp://localhost:5555");
    string workerAddr("tcp://localhost:5556");
    string dbAddr("ipc://ipc://THDatabase.ipc");
    string configFile("/etc/TeleHealth/TeleHealth.ini");

    while (true) {
        c = getopt_long(argc, argv, "c:s:d:w:",
                        options, &optionIndex);
        if (c == -1)
            break;
        switch (c)
        {
            case 0:
            {
                printf("option %s", options[optionIndex].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;
            }

            case 's':
            {
                serverAddr = optarg;
                break;
            }

            case 'c':
            {
                configFile = optarg;
                break;
            }

            case 'd':
            {
                dbAddr = optarg;
                break;
            }

            case 'w':
            {
                workerAddr = optarg;
                break;
            }
            case '?':
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    ConfigParserPtr config( NEW ConfigParser(configFile));
    if (config && !config->m_valid)
    {
        config.reset();
    }

    return config;
}

/* See description in header file. */
const string& ConfigParser::GetRootDirectory() const
{
    return m_root;
}

/* See description in header file. */
uint32 ConfigParser::GetIoThreadNumber() const
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


/* See description in header file. */
uint32 ConfigParser::GetThreadsPerNode() const
{
    return 64;
}

/* See description in header file. */
ConfigSection::ConfigSection()
{
}

/* See description in header file. */
ConfigSection::~ConfigSection()
{
}

/* See description in header file. */
int ConfigSection::ParseFromString(const string& content)
{
    return 0;
}

/* See description in header file. */
string ConfigSection::GetSectionName() const
{
    return string();
}

/* See description in header file. */
string ConfigSection::GetConfig(const string& key) const
{
    return string();
}

#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

#include <string>
#include <SmartPointer.h>

class ConfigParser;
typedef shared_ptr<ConfigParser> ConfigParserPtr;


class ConfigParser
{
public:
    virtual ~ConfigParser();
    static ConfigParserPtr GetConfigParserWithParams(int argc, char** argv);
    const string& GetRootDirectory();
    const char* GetServerPort();

private:
    ConfigParser();
    bool m_valid;
    string m_root;
};

#endif /* _CONFIGPARSER_H_ */

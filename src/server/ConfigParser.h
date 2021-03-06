#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

#include <string>
#include <SmartPointer.h>

class ConfigParser;
typedef shared_ptr<ConfigParser> ConfigParserPtr;


class ConfigParser
{
public:
    ConfigParser();
    virtual ~ConfigParser();
    static ConfigParserPtr GetConfigParserWithParams(int argc, char** argv);
    const string& GetRootDirectory();

private:
    bool m_valid;
    string m_root;
};

#endif /* _CONFIGPARSER_H_ */

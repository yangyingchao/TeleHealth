#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

#include <string>
#include <SmartPointer.h>
#include <typedefs.h>


class ConfigParser;
typedef shared_ptr<ConfigParser> ConfigParserPtr;


class ConfigParser
{
public:
    virtual ~ConfigParser();
    static ConfigParserPtr GetConfigParserWithParams(int argc, char** argv);
    const string& GetRootDirectory() const;
    const uint32 GetIoThreadNumber() const;
    const string& GetExternalAddress() const;
    const string& GetDBAddress() const;
    const string& GetDealerAddress() const;

private:
    ConfigParser();
    bool m_valid;
    string m_root;
    string m_externalAddr;
    string m_dbAddr;
    string m_dealerAddr;
};

#endif /* _CONFIGPARSER_H_ */

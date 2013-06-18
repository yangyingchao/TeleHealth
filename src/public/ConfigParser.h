#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

#include <string>
#include <SmartPointer.h>
#include <typedefs.h>

/** ConfigSection -- single section of config file.
 */
class ConfigSection
{
public:
    ConfigSection();
    virtual ~ConfigSection();

    /**
     * @name ParseFromString - Parses a string and format a ConfigSection.
     * @param content -  content of string.
     * @return 0 if succeeded, or -1 otherwise.
     */
     int ParseFromString(const string& content);

    /**
     * @name GetSectionName - return name of section.
     * @return string
     */
    string GetSectionName() const;

    /**
     * @name GetConfig - Get config value specified by key.
     * @param key -  key to find.
     * @return string
     */
    string GetConfig(const string& key) const;

private:
    string m_sectionName;               /**< Name of this section */
    map<string, string> m_kvPair;       /**< Stored key-value pairs. */
};

/** ConfigParser --  a generic ini parser.
 *
 */
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
    ConfigParser(const string& configFile);
    bool m_valid;
    string m_root;
    string m_externalAddr;
    string m_dbAddr;
    string m_dealerAddr;

    map<string, ConfigSection*> m_sections;
};

typedef shared_ptr<ConfigParser> ConfigParserPtr;

#endif /* _CONFIGPARSER_H_ */

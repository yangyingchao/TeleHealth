#include <string>
#include <sstream>

using namespace std;

class StringUtils
{
public:
    template <typename T>
    static string Number2String(T number)
    {
        stringstream ss;
        ss << number;
        return ss.str();
    }

private:
    StringUtils()
    {
    }
    virtual ~StringUtils()
    {
    }
};

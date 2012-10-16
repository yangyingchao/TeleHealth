#include "Utils.h"

#include <uuid.h>

using namespace std;

string GenerateUUID()
{
    uuid_t uu;
    uuid_generate(uu);
    char tmp[32] = {'\0'};
    uuid_unparse(uu, tmp);
    return string(tmp);
}

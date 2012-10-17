#include "Utils.h"

#ifdef __APPLE__
#include <uuid/uuid.h>
#else
#include <uuid.h>
#endif

using namespace std;

string GenerateUUID()
{
    uuid_t uu;
    uuid_generate(uu);
    char tmp[32] = {'\0'};
    uuid_unparse(uu, tmp);
    return string(tmp);
}

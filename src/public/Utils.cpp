#include "Utils.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <libgen.h>

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


/**
 * Compare two input strings to sort.
 *
 * @param p1,p2 - Strings to be compared.
 *
 * @return: int
 */
int cmpstringgp(const void *p1, const void *p2)
{
    char *pp1 = *(char * const *)p1;
    char *pp2 = *(char * const *)p2;

    while ( *pp1 == '<' || *pp1 == '=' || *pp1 == '>') {
        pp1 ++;
    }
    while ( *pp2 == '<' || *pp2 == '=' || *pp2 == '>') {
        pp2 ++;
    }

    return strcmp(pp1, pp2);
}

int dir_exist(const char *path)
{
    if (!path)
        return -1;

    int ret;
    ret = access(path, F_OK);
    if (ret < 0) 
        return -1;
    struct stat sb;
    if ((ret = stat(path, &sb)) == 0) {
        if (S_ISDIR(sb.st_mode)) 
            return 0;
    }
    return -1;
}


int file_exist(const char *path)
{
    if (!path)
        return -1;

    int ret;
    ret = access(path, F_OK);
    if (ret < 0) 
        return -1;
    struct stat sb;
    if ((ret = stat(path, &sb)) == 0) {
        if (S_ISREG(sb.st_mode)) 
            return 0;
    }
    return -1;
}

bool MakeDirectory(const char* path)
{
#ifdef _WIN32
    return false;
#else
    return (mkdir(path, 777) == 0);
#endif
}

bool MakeDirectoryEx(const char* path)
{
    char cmd[256] = {'\0'};
    sprintf(cmd, "mkdir -p %s", path);
    return system(cmd) == 0 ? true : false;
}

bool MakeDirectoryEx(const string& path)
{
    return MakeDirectoryEx(path.c_str());
}


string GetDirectorName(const char* path)
{
    string pname;
    if (path)
    {
        char* tmp = strdup(path);
        char* p = dirname(tmp);
        if (p)
        {
            pname = p;
        }
        free(tmp);
    }
    return pname;
}

string GetDirectorName(const string& path)
{
    return GetDirectorName(path.c_str());
}

#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

std::string GenerateUUID();

int dir_exist(const char *path);
int file_exist(const char *path);
int cmpstringgp(const void *p1, const void *p2);

bool MakeDirectory(const char* path);

bool MakeDirectoryEx(const char* path);
bool MakeDirectoryEx(const std::string& path);

std::string GetDirectorName(const char* path);
std::string GetDirectorName(const std::string& path);

#endif /* _UTILS_H_ */

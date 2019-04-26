#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

class FileOps
{
public:
    FileOps();
	~FileOps();
	int rows_number(string);
    FILE* open_file(string, const char *);
};

#endif

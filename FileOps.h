#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <cstring>

using namespace std;

class FileOps
{
public:
    FileOps();
	~FileOps();
	int rows_number(string);
    void open_file(FILE *, string, const char *);
};

#endif

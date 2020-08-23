#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <iostream>

class FileOps
{
public:
    explicit FileOps();
    virtual ~FileOps();
	int rowsNumber(std::string fName);
    FILE* openFile(std::string fName, const char *mode);
};

#endif

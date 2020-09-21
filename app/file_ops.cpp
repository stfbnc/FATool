#include "file_ops.h"

FileOps::FileOps() {}

FileOps::~FileOps() {}

FILE* FileOps::openFile(std::string fName, const char *mode)
{
    FILE *fl;
    fl = fopen(fName.c_str(), mode);
    return fl;
}

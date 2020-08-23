#include "file_ops.h"

FileOps::FileOps() {}

FileOps::~FileOps() {}

int FileOps::rowsNumber(std::string fName)
{
    int stop;
    int lines = 0;
    FILE *f;
    f = openFile(fName, "r");
    while(!feof(f)){
        stop = fgetc(f);
        if(stop == '\n')
            lines++;
    }
    fclose(f);
    return lines;
}

FILE* FileOps::openFile(std::string fName, const char *mode)
{
    FILE *fl;
    fl = fopen(fName.c_str(), mode);
    return fl;
}

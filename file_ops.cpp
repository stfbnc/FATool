#include "file_ops.h"

FileOps::FileOps() {}

FileOps::~FileOps() {}

int FileOps::rowsNumber(std::string fName){
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

FILE* FileOps::openFile(std::string fName, const char *mode){
    //char *fileName;
    //fileName = new char [fName.length()+1];
    //strcpy(fileName, fName.c_str());
    FILE *fl;
    //fl = fopen(fileName, mode);
    fl = fopen(fName.c_str(), mode);
    //delete fileName;
    return fl;
}

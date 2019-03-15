#ifndef FA_H
#define FA_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include "FileOps.h"

using namespace std;

class FA
{
public:
	FA() {}
	virtual ~FA() {}

    void checkFileExistence(string fn){
        struct stat buffer;
        if(stat(fn.c_str(), &buffer) != 0){
            fprintf(stdout, "ERROR %d: file %s does not exist\n", FILE_FAILURE, fn.c_str());
            exit(FILE_FAILURE);
        }
    }
    
    int setTsLength(string fn){
        FileOps fo = FileOps();
        return fo.rows_number(fn);
    }
    
    int getRangeLength(int start, int end){
        return end - start + 1;
    }
    
	virtual void allocateMemory(int, int) = 0;
	virtual void setFlucVectors() = 0;
	virtual void winFlucComp() = 0;
    virtual void H_loglogFit(int, int) = 0;
    virtual void saveFile(string) = 0;
};

#endif

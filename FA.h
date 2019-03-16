#ifndef FA_H
#define FA_H

#include "FAGlobs.h"

class FA
{
public:
	FA(){
		N = 0;
		t = NULL;
		y = NULL;
		F = NULL;
	}
	virtual ~FA(){}

	template <class T>
	void delAlloc(T *p){
		if(p != NULL)
			delete [] p;
	}

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
    
    virtual void checkInputs() = 0;
	virtual void allocateMemory() = 0;
	virtual void setFlucVectors() = 0;
	virtual void winFlucComp() = 0;
    virtual void H_loglogFit(int, int) = 0;
    virtual string outFileStr() = 0;
    virtual void saveFile(string) = 0;
protected:
    string file_name;
    int N;
    double *t;
	double *y;
	double *F;
};

#endif

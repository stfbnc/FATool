#ifndef FA_H
#define FA_H

#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include "FileOps.h"

using namespace std;

#define RANGE_FAILURE -99
#define WIN_SIZE_FAILURE -98
#define POL_FAILURE -97
#define REV_SEG_FAILURE -96
#define FILE_FAILURE -95

class FA
{
public:
    /*FA(string file_name_, int min_win_, int max_win_, int ord_, int rev_seg_=1)
	  : file_name(file_name_), min_win(min_win_), max_win(max_win_), ord(ord_), rev_seg(rev_seg_) {
          N = GetTsLength(file_name_);
      }*/
	FA() {}
	virtual ~FA() {}

    void checkFileExistence(string fn){
        struct stat buffer;
        if(stat(fn.c_str(), &buffer) != 0){
            fprintf(stdout, "ERROR %d: file %s does not exist\n", FILE_FAILURE, fn.c_str());
            exit(FILE_FAILURE);
        }
    }
    
    /*void checkInputs(int mw, int Mw, int po, int rvsg=1){
        //windows size
        if(Mw < mw){
            fprintf(stdout, "ERROR %d: biggest scale must be greater than smallest scale\n", RANGE_FAILURE);
            exit(RANGE_FAILURE);
        }else if(mw < 3){
            fprintf(stdout, "ERROR %d: smallest scale must be greater than 2\n", WIN_SIZE_FAILURE);
            exit(WIN_SIZE_FAILURE);
        }else if(Mw > N){
            fprintf(stdout, "ERROR %d: biggest scale must be smaller than time series length\n", WIN_SIZE_FAILURE);
            exit(WIN_SIZE_FAILURE);
        }
        //polynomial order
        if(po < 1){
            fprintf(stdout, "ERROR %d: polynomial order must be greater than 0\n", POL_FAILURE);
            exit(POL_FAILURE);
        }
        //rev_seg
        if(rvsg != 0 && rvsg != 1){
            fprintf(stdout, "ERROR %d: parameter for backward computation must be 0 or 1\n", REV_SEG_FAILURE);
            exit(REV_SEG_FAILURE);
        }
    }*/
    
    /*void allocateMemory(int L1, int L2){
        t = new double [L1];
        y = new double [L1];
        s = new int [L2];
        F = new double [L2];
    }*/
    
    int setTsLength(string fn){
        FileOps fo = FileOps();
        return fo.rows_number(fn);
    }
    
    int getRangeLength(int start, int end){
        return end - start + 1;
    }
    
	//virtual void checkInputs() = 0;
	virtual void allocateMemory(int, int) = 0;
	virtual void setFlucVectors() = 0;
	virtual void winFlucComp() = 0;
    virtual void H_loglogFit(int, int) = 0;
    virtual void saveFile(string) = 0;
/*protected:
	string file_name;
	int min_win;
	int max_win;
	int ord;
	int rev_seg;
    int N;
	double *t;
	double *y;
	int *s;
	double *F;
*/
};

#endif

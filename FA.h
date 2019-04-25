#ifndef FA_H
#define FA_H

#include "FAGlobs.h"
#include "qcustomplot.h"

class FA
{
public:
	FA(){
		N = 0;
        t = nullptr;
        y = nullptr;
        F = nullptr;
	}
	virtual ~FA(){}

	template <class T>
	void delAlloc(T *p){
		if(p != NULL)
			delete [] p;
	}

	template <class T>
	void del2Alloc(T **p, int iLen){
		if(p != NULL){
			for(int i = 0; i < iLen; i++)
				delAlloc<T>(p[i]);
			delete [] p;
		}
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
    
    int getRangeLength(int start, int end, int step=1){
        return (end - start) / step + 1;
    }
    
    virtual void allocateMemory() = 0;

	void setFlucVectors(){
	    MathOps mo = MathOps();
	    ArrayOps ao = ArrayOps();
	    FileOps fo = FileOps();
		//time series vector
		double pn[N], pn_nomean[N];
	    FILE *f;
	    f = fo.open_file(file_name, "r");
	    for(int i = 0; i < N; i++)
	        fscanf(f, "%lf", &pn[i]);
	    fclose(f);
		//time vector
	    ao.double_range(t, N, 1.0);
	    //time series minus its mean
	    mo.subtract_mean(pn, N, pn_nomean);
	    //cumulative sum
	    mo.cumsum(pn_nomean, y, N);
	}

	virtual void winFlucComp() = 0;
    virtual void H_loglogFit(int, int) = 0;
    virtual string outFileStr() = 0;
    virtual void saveFile(string) = 0;
    virtual void plot(QCustomPlot *) = 0;
protected:
    string file_name;
    int N;
    double *t;
	double *y;
	double *F;
};

#endif

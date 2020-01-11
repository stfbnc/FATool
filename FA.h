#ifndef FA_H
#define FA_H

#include "constants.h"
#include "base_plot.h"
#include <sys/stat.h>
#include "file_ops.h"
#include "array_ops.h"
#include "math_ops.h"

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
        if(p != nullptr)
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

    void checkFileExistence(std::string fn){
        struct stat buffer;
        if(stat(fn.c_str(), &buffer) != 0){
            fprintf(stdout, "ERROR %d: file %s does not exist\n", FILE_FAILURE, fn.c_str());
            exit(FILE_FAILURE);
        }
    }

    int getFileLength(std::string fn){
        FileOps fo = FileOps();
        return fo.rowsNumber(fn);
    }

    int getNansNumber(std::string fn){
        FileOps fo = FileOps();
        int L = getFileLength(fn);
        FILE *f;
        f = fo.openFile(fn, "r");
        int nans = 0;
        for(int i = 0; i < L; i++){
            double val;
            fscanf(f, "%lf", &val);
            if(std::isnan(val))
                nans++;
        }
        fclose(f);
        return nans;
    }

    int setTsLength(std::string fn){
        return getFileLength(fn) - getNansNumber(fn);
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
        double *pn, *pnNomean;
        pn = new double [N];
        pnNomean = new double [N];
	    FILE *f;
	    f = fo.openFile(fileName, "r");
        int idx = 0;
        for(int i = 0; i < getFileLength(fileName); i++){
            double val;
            fscanf(f, "%lf", &val);
            if(!std::isnan(val)){
                pn[idx] = val;
                idx++;
            }
        }
	    fclose(f);
		//time vector
	    ao.doubleRange(t, N, 1.0);
	    //time series minus its mean
	    mo.subtractMean(pn, N, pnNomean);
	    //cumulative sum
	    mo.cumsum(pnNomean, y, N);
        delAlloc<double>(pn);
        delAlloc<double>(pnNomean);
	}

    virtual bool computeFlucVec() = 0;
    virtual void fitFlucVec(int start, int end) = 0;
    virtual std::string outFileStr() = 0;
    virtual void saveFile(std::string pathTot) = 0;
    virtual void plot(BasePlot *plt) = 0;
protected:
    std::string fileName;
    int N;
    double *t;
	double *y;
	double *F;
};

#endif

#ifndef FA_H
#define FA_H

#include "constants.h"
#include "base_plot.h"
#include "file_ops.h"
#include "array_ops.h"
#include "math_ops.h"

class FA
{
public:
    FA(double *ts_, int tsLen_){
        ts = ts_;
        tsLen = tsLen_;
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

    int getNansNumber(){
        int nans = 0;
        for(int i = 0; i < tsLen; i++){
            if(std::isnan(ts[i]))
                nans++;
        }
        return nans;
    }

    int setTsLength(){
        return tsLen - getNansNumber();
    }
    
    int getRangeLength(int start, int end, int step=1){
        return (end - start) / step + 1;
    }
    
    virtual void allocateMemory() = 0;

	void setFlucVectors(){
	    MathOps mo = MathOps();
	    ArrayOps ao = ArrayOps();
        double *pn, *pnNomean;
        pn = new double [N];
        pnNomean = new double [N];
        int idx = 0;
        for(int i = 0; i < tsLen; i++){
            if(!std::isnan(ts[i])){
                pn[idx] = ts[i];
                idx++;
            }
        }
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
    double *ts;
    int tsLen;
    int N;
    double *t;
	double *y;
	double *F;
};

#endif

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
    FA(std::vector<double> ts, int tsLen){
        this->tsLen = tsLen;
        this->ts.reserve(tsLen);
        for(int i = 0; i < tsLen; i++)
            this->ts.push_back(ts.at(i));
		N = 0;
	}

    virtual ~FA(){}

    int getNansNumber(std::vector<double> vec, int len){
        int nans = 0;
        for(int i = 0; i < len; i++){
            if(std::isnan(vec.at(i)))
                nans++;
        }
        return nans;
    }

    int setTsLength(std::vector<double> vec, int len){
        return len - getNansNumber(vec, len);
    }
    
    int getRangeLength(int start, int end, int step=1){
        return (end - start) / step + 1;
    }
    
    virtual void allocateMemory() = 0;

	void setFlucVectors(){
	    MathOps mo = MathOps();
	    ArrayOps ao = ArrayOps();
        std::vector<double> pn, pnNomean, pnNoNan;
        pn.reserve(N);
        pnNomean.reserve(N);
        pnNoNan.reserve(setTsLength(ts, tsLen));
        ao.noNan(ts, tsLen, pnNoNan);
        ao.sliceVec(pnNoNan, pn, 0, N-1);
		//time vector
        ao.doubleRange(t, N, 1.0);
	    //time series minus its mean
        mo.subtractMean(pn, N, pnNomean);
	    //cumulative sum
	    mo.cumsum(pnNomean, y, N);
	}

    virtual bool computeFlucVec() = 0;
    virtual void fitFlucVec(int start, int end) = 0;
    virtual std::string outFileStr() = 0;
    virtual void saveFile(std::string pathTot) = 0;
    virtual void plot(BasePlot *plt) = 0;
protected:
    std::string fileName;
    std::vector<double> ts;
    int tsLen;
    int N;
    std::vector<double> t;
    std::vector<double> y;
    std::vector<double> F;
};

#endif

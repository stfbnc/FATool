#ifndef HTsingleScale_H
#define HTsingleScale_H

#include "FA.h"
#include "MFDFA_single_q.h"

class HTsingleScale : public FA
{
public:
    HTsingleScale(std::string fileName_, double *ts_, int tsLen_, int scale_);
	~HTsingleScale();
    void allocateMemory() override;
	int getTsLength();
    std::string getFileName();
    bool computeFlucVec() override;
    void setMFDFAstep(int mfdfaStep);
    void fitFlucVec(int start, int end) override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
protected:
	int scale;
	int step;
	double *Ht;
};

#endif

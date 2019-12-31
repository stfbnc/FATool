#ifndef MFDFA_H
#define MFDFA_H

#include "MFDFA_single_q.h"

class MFDFA : public MFDFAsingleQ
{
public:
    MFDFA(std::string fileName_, int minWin_, int maxWin_, int ord_, double qIn_, int Nq_, int winStep_=1, double stepq_=1.0, int revSeg_=1);
    ~MFDFA() override;
    void allocateQmemory();
    void setQrange(double, int, double);
    bool computeFlucVec() override;
	std::string outFileStr() override;
	void saveFile(std::string pathTot) override;
	std::string qoutFileStr();
	void qsaveFile(std::string pathTot);
    void plot(BasePlot *plt) override;
private:
    int Nq;
    double stepq;
    double *qRange;
	double **flucMtx;
    double *Hq;
    double *Hinterceptq;
};

#endif

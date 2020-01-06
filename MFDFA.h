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
    void computeMassExponents();
    void computeSpectrum();
	std::string outFileStr() override;
	void saveFile(std::string pathTot) override;
	std::string qoutFileStr();
	void qsaveFile(std::string pathTot);
    std::string tauOutFileStr();
    void tauSaveFile(std::string pathTot);
    std::string spectrumOutFileStr();
    void spectrumSaveFile(std::string pathTot);
    void plot(BasePlot *plt) override;
    void plotMassExponents(BasePlot *plt);
    void plotSpectrum(BasePlot *plt);
private:
    int Nq;
    double stepq;
    double *qRange;
	double **flucMtx;
    double *Hq;
    double *Hinterceptq;
    double *tau;
    double *alpha;
    double *spectrum;
};

#endif

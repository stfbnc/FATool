#ifndef MFDFA_H
#define MFDFA_H

#include "MFDFA_single_q.h"

class MFDFA : public MFDFAsingleQ
{
public:
    MFDFA(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, double qIn, int Nq, int winStep=1, double stepq=1.0, int revSeg=1);
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
    std::vector<double> qRange;
    std::vector<std::vector<double>> flucMtx;
    std::vector<double> Hq;
    std::vector<double> Hinterceptq;
    std::vector<double> tau;
    std::vector<double> alpha;
    std::vector<double> spectrum;
};

#endif

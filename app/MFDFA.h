#ifndef MFDFA_H
#define MFDFA_H

#include "MFDFA_single_q.h"

class MFDFA : public MFDFAsingleQ
{
Q_OBJECT
public:
    explicit MFDFA(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, double qIn, int Nq, int winStep=1, double stepq=1.0, int revSeg=1);
    virtual ~MFDFA();
    void computeMassExponents();
    void computeSpectrum();
	std::string outFileStr() override;
	void saveFile(std::string pathTot) override;
    void tauSaveFile(std::string pathTot);
    void spectrumSaveFile(std::string pathTot);
    void plot(BasePlot *plt) override;
    void plotMassExponents(BasePlot *plt);
    void plotSpectrum(BasePlot *plt);
    int getAlgorithmTotalSteps() override;
    std::string getCurrentIdentifier() override;
public slots:
    void executeAlgorithm() override;
private:
    void setQrange(double, int, double);
    std::string qoutFileStr();
    void qsaveFile(std::string pathTot);
    std::string tauOutFileStr();
    std::string spectrumOutFileStr();

    int Nq;
    double stepq;
    std::vector<double> qRange = std::vector<double>();
    std::vector<std::vector<double>> flucMtx = std::vector<std::vector<double>>();
    std::vector<double> Hq = std::vector<double>();
    std::vector<double> Hinterceptq = std::vector<double>();
    std::vector<double> tau = std::vector<double>();
    std::vector<double> alpha = std::vector<double>();
    std::vector<double> spectrum = std::vector<double>();
};

#endif

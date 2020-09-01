#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "DCCA.h"

class rhoDCCA : public FA
{
public:
    explicit rhoDCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, int winStep=1, bool thresh=false);
    virtual ~rhoDCCA();
    QString getAlgorithmStr() override;
    bool executeAlgorithm() override;
    void computeThresholds();
    bool threshCompute();
    std::string getFileName1();
    std::string getFileName2();
    int getMinWin() override;
    int getMaxWin() override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
private:
    void confLevels(std::vector<double> rhos);
    int getRhoLength();

    std::string fileName;
	std::string fileName2;
    std::vector<double> ts2 = std::vector<double>();
    int tsLen2;
	int ord;
	int winStep;
    bool thresh;
    std::vector<double> rho = std::vector<double>();
    std::vector<double> confDown = std::vector<double>();
    std::vector<double> confUp = std::vector<double>();
    int L;
    int N;

    const int nSim = 50;
    const double confLev = 0.95;
};

#endif

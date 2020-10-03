#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "DCCA.h"

class rhoDCCA : public FA
{
Q_OBJECT
public:
    explicit rhoDCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, int winStep=1, bool thresh=false);
    virtual ~rhoDCCA();
    QString getAlgorithmStr() override;
    void computeThresholds();
    bool threshCompute();
    std::string getFileName() override;
    std::string getFileName2() override;
    int getMinWin() override;
    int getMaxWin() override;
    bool isFittable() override;
    int getLogType() override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
    int getAlgorithmTotalSteps() override;
    std::string getCurrentIdentifier() override;
public slots:
    void executeAlgorithm() override;
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

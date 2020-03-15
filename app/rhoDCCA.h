#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "DCCA.h"

class rhoDCCA
{
public:
    rhoDCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, int winStep=1, bool thresh=false);
	~rhoDCCA();
    bool computeRho();
    void computeThresholds();
    void confLevels(std::vector<double> rhos);
    int getRhoLength();
    bool threshCompute();
    std::string getFileName1();
    std::string getFileName2();
    std::string outFileStr();
    void saveFile(std::string pathTot);
    void plot(BasePlot *plt);
private:
	std::string fileName;
    std::vector<double> ts;
    int tsLen;
	std::string fileName2;
    std::vector<double> ts2;
    int tsLen2;
	int minWin;
	int maxWin;
	int ord;
	int winStep;
    bool thresh;
    std::vector<double> rho;
    std::vector<double> confDown;
    std::vector<double> confUp;
    int L;
    int N;

    const int nSim = 50;
    const double confLev = 0.95;
};

#endif

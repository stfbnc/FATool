#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "DCCA.h"

class rhoDCCA
{
public:
    rhoDCCA(std::string fileName_, double *ts_, int tsLen_, std::string fileName2_, double *ts2_, int tsLen2_, int minWin_, int maxWin_, int ord_, int winStep_=1, bool thresh=false);
	~rhoDCCA();
    bool computeRho();
    void computeThresholds();
    void confLevHist(double *rhos);
    int getRhoLength();
    bool threshCompute();
    std::string getFileName1();
    std::string getFileName2();
    std::string outFileStr();
    void saveFile(std::string pathTot);
    void plot(BasePlot *plt);
private:
	std::string fileName;
    double *ts;
    int tsLen;
	std::string fileName2;
    double *ts2;
    int tsLen2;
	int minWin;
	int maxWin;
	int ord;
	int winStep;
    bool thresh;
    double *rho;
    double *confDown;
    double *confUp;
    int L;
    int N;

    const int nSim = 50;
    const double confLev = 0.95;
};

#endif

#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "DCCA.h"

class rhoDCCA
{
public:
    rhoDCCA(std::string fileName_, std::string fileName2_, int minWin_, int maxWin_, int ord_, int winStep_=1);
	~rhoDCCA();
    bool computeRho();
    int getRhoLength();
    std::string getFileName1();
    std::string getFileName2();
    std::string outFileStr();
    void saveFile(std::string pathTot);
    void plot(BasePlot *plt);
private:
	std::string fileName;
	std::string fileName2;
	int minWin;
	int maxWin;
	int ord;
	int winStep;
    double *rho;
    int L;
};

#endif

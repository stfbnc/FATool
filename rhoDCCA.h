#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "FAGlobs.h"
#include "DCCA.h"

class rhoDCCA
{
public:
    rhoDCCA(string, string, int, int, int);
	~rhoDCCA();
    void computeRho();
    int getRhoLength();
    string outFileStr();
    void saveFile(string);
    //void plot();
private:
	string file_name;
	string file_name2;
	int min_win;
	int max_win;
	int ord;
    double *rho;
    int L;
};

#endif

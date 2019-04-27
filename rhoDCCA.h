#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "FAGlobs.h"
#include "DCCA.h"
#include <QtWidgets>

class rhoDCCA
{
public:
    rhoDCCA(string, string, int, int, int, int=1);
	~rhoDCCA();
    void computeRho();
    int getRhoLength();
    string outFileStr();
    void saveFile(string);
    void plot(QCustomPlot *);
private:
	string file_name;
	string file_name2;
	int min_win;
	int max_win;
	int ord;
	int win_step;
    double *rho;
    int L;
};

#endif

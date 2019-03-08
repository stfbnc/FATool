#ifndef MFDFA_H
#define MFDFA_H

#include <iostream>
#include <cstring>
#include "MFDFAsingleQ.h"

using namespace std;

class MFDFA : public MFDFAsingleQ
{
public:
    MFDFA(string, int, int, int, double, int, double=1.0, int=1);
	~MFDFA();
    void allocateQmemory(int, int);
    void getQrange(double, int, double);
	void qWinFlucComp();
	void saveFile(string path_tot);
	void qsaveFile(string path_tot);
	//void plot();
	//void qplot();
private:
    int Nq;
    double stepq;
    double *qRange;
	double **flucMtx;
    double *Hq;
    double *H_interceptq;
};

#endif

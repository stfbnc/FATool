#ifndef MFDFA_H
#define MFDFA_H

#include "FAGlobs.h"
#include "MFDFAsingleQ.h"

class MFDFA : public MFDFAsingleQ
{
public:
    MFDFA(string, int, int, int, double, int, double=1.0, int=1);
	~MFDFA();
	void checkInputs() override;
    void allocateQmemory();
    void setQrange(double, int, double);
	void winFlucComp() override;
	string outFileStr() override;
	void saveFile(string) override;
	string qoutFileStr();
	void qsaveFile(string);
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

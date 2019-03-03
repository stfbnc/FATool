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
    void allocateQmemory(int);
    void getQrange(double, int, double);
	//void plot();
private:
	string file_name;
	int min_win;
	int max_win;
	int ord;
    double q;
	int rev_seg;
    int N;
    int Nq;
    double stepq;
    double *qRange;
	double *t;
	double *y;
	int *s;
	double *F;
	double H;
	double H_intercept;
    double *Hq;
    double *H_interceptq;
};

#endif

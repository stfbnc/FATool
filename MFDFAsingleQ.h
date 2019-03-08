#ifndef MFDFAsingleQ_H
#define MFDFAsingleQ_H

#include <iostream>
#include <cstring>
#include "FA.h"

using namespace std;

class MFDFAsingleQ : public FA
{
public:
    MFDFAsingleQ(string, int, int, int, double, int=1);
	~MFDFAsingleQ();
	void checkInputs(int, int, int, int);
	void allocateMemory(int, int);
	int getTsLength();
	void setFlucVectors();
	void winFlucComp();
	double getH();
	double getH_intercept();
	void H_loglogFit(int, int);
	void saveFile(string);
	//void plot();
//private:
protected:
	string file_name;
	int min_win;
	int max_win;
	int ord;
    double q;
	int rev_seg;
    int N;
	double *t;
	double *y;
	int *s;
	double *F;
	double H;
	double H_intercept;
};

#endif

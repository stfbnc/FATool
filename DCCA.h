#ifndef DCCA_H
#define DCCA_H

#include <iostream>
#include <cstring>
#include "FA.h"

#define DEFAULT_DCCA "abs"
#define CORR_DCCA "sign"

using namespace std;

class DCCA : public FA
{
public:
    DCCA(string, string, int, int, int, string=DEFAULT_DCCA);
	~DCCA();
	void checkInputs(int, int, int, string);
	void allocateMemory(int, int);
    void getEqualLength(string, string);
	int getTsLength();
	void setFlucVectors();
	void winFlucComp();
    double *getF();
	double getH();
	double getH_intercept();
	void H_loglogFit(int, int);
	void saveFile(string);
    //void plot();
private:
	string file_name;
	string file_name2;
	int min_win;
	int max_win;
	int ord;
    string isAbs;
    int N;
	double *t;
	double *y;
	double *y2;
	int *s;
	double *F;
	double H;
	double H_intercept;
};

#endif

#ifndef DCCA_H
#define DCCA_H

#include "FA.h"

class DCCA : public FA
{
public:
    DCCA(string, string, int, int, int);
	~DCCA();
    void getEqualLength(string, string);
	void checkInputs(int, int, int);
	void allocateMemory(int, int);
	int getTsLength();
	void setFlucVectors();
	void winFlucComp();
	double getH();
	double getH_intercept();
	void H_loglogFit(int, int);
	void saveFile(string);
private:
	string file_name;
	string file_name2;
	int min_win;
	int max_win;
	int ord;
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

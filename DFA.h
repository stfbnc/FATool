#ifndef DFA_H
#define DFA_H

#include "FA.h"

class DFA : public FA
{
public:
    DFA(string, int, int, int, int=1);
	~DFA();
	void checkInputs(int, int, int, int);
	void allocateMemory(int, int);
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
	int min_win;
	int max_win;
	int ord;
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

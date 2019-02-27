#ifndef DFA_H
#define DFA_H

#include "FA.h"

class DFA : public FA
{
public:
    DFA(string, int, int, int, int=1);
	~DFA();
	void SetFlucVectors();
	void WinFlucComp();
	double H_loglogFit(int, int);
	void SaveFile(string);
	void setH(double);
	double getH();
	void setH_intercept(double);
	double getH_intercept();
private:
	double H;
	double H_intercept;
};

#endif

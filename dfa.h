#ifndef DFA_H
#define DFA_H

#include "FA.h"

class DFA : public FA
{
public:
    DFA(string, int, int, int, int);
	~DFA();
	void SetFlucVectors();
	void WinFlucComp();
	double H_loglogFit(int, int);
	void SaveFile(string);
};

#endif

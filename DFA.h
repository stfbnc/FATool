#ifndef DFA_H
#define DFA_H

#include "FAGlobs.h"
#include "FA.h"

class DFA : public FA
{
public:
    DFA(string, int, int, int, int=1, int=1);
	~DFA();
	void checkInputs();
	void allocateMemory();
	int getTsLength();
	void winFlucComp();
    int getWinStep();
	double getH();
	double getH_intercept();
	void H_loglogFit(int, int);
	string outFileStr();
	void saveFile(string);
    void plot(QCustomPlot *);
private:
	int min_win;
	int max_win;
	int ord;
	int rev_seg;
	int win_step;
	int *s;
	double H;
	double H_intercept;
};

#endif

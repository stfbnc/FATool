#ifndef MFDFAsingleQ_H
#define MFDFAsingleQ_H

#include "FAGlobs.h"
#include "FA.h"
#include <QtWidgets>

class MFDFAsingleQ : public FA
{
public:
    MFDFAsingleQ(string, int, int, int, double, int=1, int=1);
	~MFDFAsingleQ();
	void allocateMemory();
	int getTsLength();
    bool winFlucComp();
	double getH();
	double getH_intercept();
	void H_loglogFit(int, int);
	string outFileStr();
	void saveFile(string);
    void plot(QCustomPlot *);
protected:
	int min_win;
	int max_win;
	int ord;
    double q;
	int rev_seg;
	int win_step;
	int *s;
	double H;
	double H_intercept;
};

#endif

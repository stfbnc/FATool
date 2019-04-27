#ifndef DCCA_H
#define DCCA_H

#include "FAGlobs.h"
#include "FA.h"
#include <QtWidgets>

class DCCA : public FA
{
public:
    DCCA(string, string, int, int, int, string, int);
	~DCCA();
	void allocateMemory();
    void getEqualLength(string, string);
	int getTsLength();
	void setFlucVectors();
	void winFlucComp();
    double *getF();
	double getH();
	double getH_intercept();
	void H_loglogFit(int, int);
	string outFileStr();
	void saveFile(string);
    void plot(QCustomPlot *);
private:
	string file_name2;
	int min_win;
	int max_win;
	int ord;
    string isAbs;
    int win_step;
	double *y2;
	int *s;
	double H;
	double H_intercept;
};

#endif

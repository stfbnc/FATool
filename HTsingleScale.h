#ifndef HTsingleScale_H
#define HTsingleScale_H

#include "FAGlobs.h"
#include "FA.h"
#include <QtWidgets>

class HTsingleScale : public FA
{
public:
	HTsingleScale(string, int);
	~HTsingleScale();
	void allocateMemory();
	int getTsLength();
    bool winFlucComp();
    void H_loglogFit(int , int);
    void Ht_fit();
	string outFileStr();
	void saveFile(string);
    void plot(QCustomPlot *);
protected:
	int scale;
	double *Ht;
};

#endif

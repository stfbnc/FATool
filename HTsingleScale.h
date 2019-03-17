#ifndef HTsingleScale_H
#define HTsingleScale_H

#include "FAGlobs.h"
#include "FA.h"

class HTsingleScale : public FA
{
public:
	HTsingleScale(string, int);
	~HTsingleScale();
	void checkInputs();
	void allocateMemory();
	int getTsLength();
	void winFlucComp();
	void H_loglogFit(int, int);
	string outFileStr();
	void saveFile(string);
protected:
	int scale;
	double *Ht;
};

#endif

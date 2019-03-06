#ifndef HTsingleScale_H
#define HTsingleScale_H

#include <iostream>
#include <cstring>
#include "FA.h"

using namespace std;

class HT : public FA
{
public:
    HT(string, int, int);
	~HT();
	void setFlucVectors();
	void winFlucComp();
	double H_loglogFit(int, int);
	void saveFile(string);
protected:
	double *t;
	double *y;
	double *F;
	int scale;
	double *Ht;
};

#endif

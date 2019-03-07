#ifndef HTsingleScale_H
#define HTsingleScale_H

#include <iostream>
#include <cstring>
#include "FA.h"

using namespace std;

class HTsingleScale : public FA
{
public:
	HTsingleScale(string, int, int);
	~HTsingleScale();
	void checkInputs(int, int);
	void allocateMemory(int, int);
	int getTsLength();
	void setFlucVectors();
	void winFlucComp();
	void H_loglogFit(int, int);
	void saveFile(string);
protected:
	string file_name;
	int scale;
	int ord;
	int N;
	double *t;
	double *y;
	double *F;
	double *Ht;
};

#endif

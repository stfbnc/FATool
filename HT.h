#ifndef HT_H
#define HT_H

#include "FA.h"

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
	int scale;
	double *Ht;
};

#endif

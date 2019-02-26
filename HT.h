#ifndef HT_H
#define HT_H

#include "FA.h"

class HT : public FA
{
public:
    HT(string, int, int);
	~HT();
	void SetFlucVectors();
	void WinFlucComp();
	double H_loglogFit(int, int);
	void SaveFile(string);
protected:
	int scale;
};

#endif

#ifndef DCCA_H
#define DCCA_H

#include "FA.h"

class DCCA : public FA
{
public:
    DCCA(string, string, int, int, int);
	~DCCA();
    void GetEqualLength(int, string);
	void SetFlucVectors();
	void WinFlucComp();
	double H_loglogFit(int, int);
	void SaveFile(string);
protected:
    string file_name2;
    double *y2;
};

#endif

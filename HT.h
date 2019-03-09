#ifndef HT_H
#define HT_H

#include <iostream>
#include <cstring>
#include "HTsingleScale.h"

using namespace std;

class HT : public HTsingleScale
{
public:
	HT(string, int, int, int, int);
	//HT(string, string, int);
	~HT();
	void checkScalesInputs(int, int);
	void allocateScalesMemory(int, int);
	void scalesWinFlucComp();
	void saveFile(string) override;
private:
	int Nscales;
	int *scales;
	double **HTmtx;
};

#endif

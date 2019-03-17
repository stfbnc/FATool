#ifndef HT_H
#define HT_H

#include "FAGlobs.h"
#include "HTsingleScale.h"

class HT : public HTsingleScale
{
public:
	HT(string, int, int, int);
	HT(string, string);
	~HT();
	void checkInputs() override;
	void allocateScalesMemory();
	int getNumScales(string);
	void getScales(string);
	//void scalesWinFlucComp();
	void winFlucComp() override;
	string outFileStr() override;
	void saveFile(string) override;
private:
	int Nscales;
	int minScale;
	int stepScale;
	int *scales;
	double **HTmtx;
};

#endif

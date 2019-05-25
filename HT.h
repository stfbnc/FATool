#ifndef HT_H
#define HT_H

#include "FAGlobs.h"
#include "HTsingleScale.h"

class HT : public HTsingleScale
{
public:
	HT(string, int, int, int);
	HT(string, string);
    ~HT() override;
	void allocateScalesMemory();
	int getNumScales(string);
    int getMinScale(string);
	void getScales(string);
    bool winFlucComp() override;
	string outFileStr() override;
	void saveFile(string) override;
    void plot(QCustomPlot *) override;
private:
	int Nscales;
	int minScale;
	int stepScale;
	int *scales;
	double **HTmtx;
};

#endif

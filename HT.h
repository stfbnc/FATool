#ifndef HT_H
#define HT_H

#include "HT_single_scale.h"

class HT : public HTsingleScale
{
public:
    HT(std::string fileName_, double *ts_, int tsLen_, int scale_, int Nscales_, int stepScale_, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
    HT(std::string fileName, double *ts_, int tsLen_, std::string strScales, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
    ~HT() override;
	void allocateScalesMemory();
	int getNumScales(std::string str);
    int getMinScale(std::string str);
	void getScales(std::string str);
    bool computeFlucVec() override;
	std::string outFileStr() override;
	void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
private:
	int mMFDFA;
	int MMFDFA;
	int Nscales;
	int minScale;
	int stepScale;
	int *scales;
	double **HTmtx;
};

#endif

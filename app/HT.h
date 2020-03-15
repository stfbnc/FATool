#ifndef HT_H
#define HT_H

#include "HT_single_scale.h"

class HT : public HTsingleScale
{
public:
    HT(std::string fileName, std::vector<double> ts, int tsLen, int scale, int Nscales, int stepScale, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
    HT(std::string fileName, std::vector<double> ts, int tsLen, std::string strScales, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
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
    int mfdfaMinWin;
    int mfdfaMaxWin;
	int Nscales;
	int minScale;
	int stepScale;
    std::vector<int> scales;
    std::vector<std::vector<double>> HTmtx;
};

#endif

#ifndef HT_H
#define HT_H

#include "HT_single_scale.h"

class HT : public HTsingleScale
{
public:
    explicit HT(std::string fileName, std::vector<double> ts, int tsLen, int scale, int Nscales, int stepScale, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
    explicit HT(std::string fileName, std::vector<double> ts, int tsLen, std::string strScales, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
    virtual ~HT();
    bool executeAlgorithm() override;
	std::string outFileStr() override;
	void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
private:
    void getScales(std::string str);
    int getNumScales(std::string str);
    int getMinScale(std::string str);

    int mfdfaMinWin;
    int mfdfaMaxWin;
	int Nscales;
	int minScale;
	int stepScale;
    std::vector<int> scales = std::vector<int>();
    std::vector<std::vector<double>> HTmtx = std::vector<std::vector<double>>();
};

#endif

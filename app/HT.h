#ifndef HT_H
#define HT_H

#include "HT_single_scale.h"

class HT : public HTsingleScale
{
Q_OBJECT
    friend class FaTest;
public:
    explicit HT(std::string fileName, std::vector<double> ts, int tsLen, int scale, int Nscales, int stepScale, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
    explicit HT(std::string fileName, std::vector<double> ts, int tsLen, std::string strScales, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep);
    virtual ~HT();
	std::string outFileStr() override;
	void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
    int getAlgorithmTotalSteps() override;
    std::string getCurrentIdentifier() override;
public slots:
    void executeAlgorithm() override;
private:
    void getScales(std::string str);
    int getNumScales(std::string str);
    int getMinScale(std::string str);
    void updateProgress(int val, int n);

    int mfdfaMinWin;
    int mfdfaMaxWin;
	int Nscales;
	int minScale;
	int stepScale;
    std::vector<int> scales = std::vector<int>();
    std::vector<std::vector<double>> HTmtx = std::vector<std::vector<double>>();
};

#endif

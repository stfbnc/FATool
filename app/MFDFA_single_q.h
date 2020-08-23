#ifndef MFDFAsingleQ_H
#define MFDFAsingleQ_H

#include "FA.h"

class MFDFAsingleQ : public FA
{
public:
    explicit MFDFAsingleQ(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, double q, int winStep=1, int revSeg=1);
    virtual ~MFDFAsingleQ();
    void allocateMemory() override;
	int getTsLength();
    bool computeFlucVec() override;
    std::string getFileName();
	double getH();
	double getHintercept();
    void fitFlucVec(int start, int end) override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
protected:
	int minWin;
	int maxWin;
	int ord;
    double q;
	int revSeg;
	int winStep;
    std::vector<int> s;
	double H;
	double Hintercept;
};

#endif

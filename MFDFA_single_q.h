#ifndef MFDFAsingleQ_H
#define MFDFAsingleQ_H

#include "FA.h"

class MFDFAsingleQ : public FA
{
public:
    MFDFAsingleQ(std::string fileName_, int minWin_, int maxWin_, int ord_, double q_, int winStep_=1, int revSeg_=1);
	~MFDFAsingleQ();
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
	int *s;
	double H;
	double Hintercept;
};

#endif

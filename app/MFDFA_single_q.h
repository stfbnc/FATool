#ifndef MFDFAsingleQ_H
#define MFDFAsingleQ_H

#include "FA.h"

class MFDFAsingleQ : public FA
{
public:
    explicit MFDFAsingleQ(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, double q, int winStep=1, int revSeg=1);
    virtual ~MFDFAsingleQ();
    QString getAlgorithmStr() override;
    int getTsLength() override;
    bool executeAlgorithm() override;
    std::string getFileName();
    int getMinWin() override;
    int getMaxWin() override;
	double getH();
	double getHintercept();
    void executeFit(int start, int end) override;
    bool isFittable() override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
protected:
	int ord;
    double q;
	int revSeg;
	int winStep;
    std::vector<int> s = std::vector<int>();
	double H;
	double Hintercept;
};

#endif

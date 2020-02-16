#ifndef DFA_H
#define DFA_H

#include "FA.h"

class DFA : public FA
{
public:
    DFA(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, int winStep=1, int revSeg=1);
	~DFA();
    void allocateMemory() override;
	int getTsLength();
    bool computeFlucVec() override;
    std::string getFileName();
    int getMinWin();
    int getMaxWin();
    int getWinStep();
	double getH();
    double getHintercept();
    void fitFlucVec(int start, int end) override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
private:
	int minWin;
	int maxWin;
	int ord;
	int revSeg;
	int winStep;
    std::vector<int> s;
	double H;
	double Hintercept;
};

#endif

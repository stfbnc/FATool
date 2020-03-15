#ifndef DCCA_H
#define DCCA_H

#include "FA.h"

class DCCA : public FA
{
public:
    DCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, std::string isAbs, int winStep, bool showProgBar=true);
	~DCCA();
    void allocateMemory() override;
    void getEqualLength();
	int getTsLength();
	void setFlucVectors();
    bool computeFlucVec() override;
    std::string getFileName1();
    std::string getFileName2();
    std::vector<double> getF();
    int getMinWin();
    int getMaxWin();
	double getH();
	double getHintercept();
    void fitFlucVec(int start, int end) override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
private:
	std::string fileName2;
    std::vector<double> ts2;
    int tsLen2;
	int minWin;
	int maxWin;
	int ord;
    std::string isAbs;
    int winStep;
    std::vector<double> y2;
    std::vector<int> s;
	double H;
	double Hintercept;
    bool showProgBar;
};

#endif

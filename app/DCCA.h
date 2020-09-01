#ifndef DCCA_H
#define DCCA_H

#include "FA.h"

class DCCA : public FA
{
public:
    explicit DCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, std::string isAbs, int winStep, bool showProgBar=true);
    virtual ~DCCA();
    QString getAlgorithmStr() override;
    int getTsLength() override;
    void setVectors() override;
    bool executeAlgorithm() override;
    std::string getFileName1();
    std::string getFileName2();
    std::vector<double> getF();
    int getMinWin() override;
    int getMaxWin() override;
	double getH();
    double getHintercept();
    void executeFit(int start, int end) override;
    bool isFittable() override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
private:
    void getEqualLength();

	std::string fileName2;
    std::vector<double> ts2 = std::vector<double>();
    int tsLen2;
	int ord;
    std::string isAbs;
    int winStep;
    std::vector<double> y2 = std::vector<double>();
    std::vector<int> s = std::vector<int>();
	double H;
	double Hintercept;
    bool showProgBar;
};

#endif

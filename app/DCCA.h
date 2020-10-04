#ifndef DCCA_H
#define DCCA_H

#include "FA.h"

class DCCA : public FA
{
Q_OBJECT
public:
    explicit DCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, std::string isAbs, int winStep, bool showProgBar=true);
    virtual ~DCCA();
    QString getAlgorithmStr() override;
    int getTsLength() override;
    void setVectors() override;
    std::string getFileName() override;
    std::string getFileName2() override;
    std::vector<double> getF();
    int getMinWin() override;
    int getMaxWin() override;
    double getH() override;
    double getHintercept() override;
    void executeFit(int start, int end) override;
    bool isFittable() override;
    int getLogType() override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
    int getAlgorithmTotalSteps() override;
    std::string getCurrentIdentifier() override;
public slots:
    void executeAlgorithm() override;
signals:
    void progressSingle(int);
private:
    void getEqualLength();

    std::vector<double> ts2 = std::vector<double>();
    int tsLen2;
	int ord;
    std::string isAbs;
    int winStep;
    std::vector<double> y2 = std::vector<double>();
    std::vector<int> s = std::vector<int>();
    bool showProgBar;
};

#endif

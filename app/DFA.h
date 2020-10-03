#ifndef DFA_H
#define DFA_H

#include "FA.h"

class DFA : public FA
{
Q_OBJECT
public:
    explicit DFA(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, int winStep=1, int revSeg=1);
    virtual ~DFA();
    QString getAlgorithmStr() override;
    int getTsLength() override;
    //bool executeAlgorithm() override;
    std::string getFileName() override;
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
private:
	int ord;
	int revSeg;
	int winStep;
    std::vector<int> s = std::vector<int>();
};

#endif

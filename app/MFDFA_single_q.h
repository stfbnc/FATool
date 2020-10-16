#ifndef MFDFAsingleQ_H
#define MFDFAsingleQ_H

#include "FA.h"

class MFDFAsingleQ : public FA
{
Q_OBJECT
public:
    explicit MFDFAsingleQ(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, double q, int winStep=1, int revSeg=1);
    virtual ~MFDFAsingleQ();
    QString getAlgorithmStr() override;
    int getTsLength() override;
    std::string getFileName() override;
    int getMinWin() override;
    int getMaxWin() override;
    double getH() override;
    double getHintercept() override;
    std::vector<double> getF();
    void executeFit(int start, int end) override;
    bool isFittable() override;
    int getLogType() override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
public slots:
    void executeAlgorithm() override;
signals:
    void progressSingle(int);
protected:
	int ord;
    double q;
	int revSeg;
	int winStep;
    std::vector<int> s = std::vector<int>();
};

#endif

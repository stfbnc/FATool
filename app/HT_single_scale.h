#ifndef HTsingleScale_H
#define HTsingleScale_H

#include "FA.h"
#include "MFDFA_single_q.h"

class HTsingleScale : public FA
{
Q_OBJECT
public:
    explicit HTsingleScale(std::string fileName, std::vector<double> ts, int tsLen, int scale);
    virtual ~HTsingleScale();
    QString getAlgorithmStr() override;
    int getTsLength() override;
    std::string getFileName() override;
    void executeFit(int start, int end) override;
    bool isFittable() override;
    int getLogType() override;
    std::string outFileStr() override;
    void saveFile(std::string pathTot) override;
    void plot(BasePlot *plt) override;
public slots:
    void executeAlgorithm() override;
signals:
    void progress(int) override;
    void executionEnded(FA*) override;
protected:
    void setMFDFAstep(int mfdfaStep);

	int scale;
	int step;
    std::vector<double> Ht = std::vector<double>();
};

#endif

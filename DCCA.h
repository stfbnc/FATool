#ifndef DCCA_H
#define DCCA_H

#include "FA.h"

class DCCA : public FA
{
public:
    DCCA(std::string fileName_, std::string fileName2_, int minWin_, int maxWin_, int ord_, std::string isAbs_, int winStep_);
	~DCCA();
    void allocateMemory() override;
    void getEqualLength(std::string fn1, std::string fn2);
	int getTsLength();
	void setFlucVectors();
    bool computeFlucVec() override;
    std::string getFileName1();
    std::string getFileName2();
    double *getF();
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
	int minWin;
	int maxWin;
	int ord;
    std::string isAbs;
    int winStep;
	double *y2;
	int *s;
	double H;
	double Hintercept;
};

#endif

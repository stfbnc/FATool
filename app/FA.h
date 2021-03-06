#ifndef FA_H
#define FA_H

#include "constants.h"
#include "base_plot.h"
#include "file_ops.h"
#include "array_ops.h"
#include "math_ops.h"

class FA : public QObject
{
Q_OBJECT
public:
    explicit FA(std::vector<double> ts, int tsLen)
    {
        this->tsLen = tsLen;
        this->ts = ts;
	}

    virtual ~FA(){}

    int getNansNumber(std::vector<double> vec, int len)
    {
        int nans = 0;
        for(int i = 0; i < len; i++)
        {
            if(std::isnan(vec.at(i)))
                nans++;
        }
        return nans;
    }

    int setTsLength(std::vector<double> vec, int len)
    {
        return len - getNansNumber(vec, len);
    }

    virtual int getTsLength()
    {
        return N;
    }
    
    int getRangeLength(int start, int end, int step=1)
    {
        return (end - start) / step + 1;
    }

    virtual int getMinWin()
    {
        return minWin;
    }

    virtual int getMaxWin()
    {
        return maxWin;
    }

    virtual double getH()
    {
        return H;
    }

    virtual double getHintercept()
    {
        return Hintercept;
    }

    virtual QString getAlgorithmStr()
    {
        return "";
    }

    virtual std::string getFileName()
    {
        return fileName;
    }

    virtual std::string getFileName2()
    {
        return fileName2;
    }

    virtual void setVectors()
    {
	    MathOps mo = MathOps();
	    ArrayOps ao = ArrayOps();
        std::vector<double> pn = std::vector<double>();
        std::vector<double> pnNomean = std::vector<double>();
        std::vector<double> pnNoNan = std::vector<double>();
        ao.noNan(ts, tsLen, pnNoNan);
        ao.sliceVec(pnNoNan, pn, 0, N - 1);
        ao.doubleRange(t, N, 1.0);
        mo.subtractMean(pn, N, pnNomean);
	    mo.cumsum(pnNomean, y, N);
	}

    virtual void executeFit(int start, int end)
    {
        start = 0;
        end = 0;
    }

    virtual bool isFittable()
    {
        return false;
    }

    virtual int getLogType()
    {
        return BasePlot::NO_AX;
    }

    virtual std::string outFileStr()
    {
        return "";
    }

    virtual void saveFile(std::string pathTot)
    {
        pathTot = "";
    }

    virtual void plot(BasePlot *plt)
    {
        plt = nullptr;
    }

    virtual int getAlgorithmTotalSteps()
    {
        return 0;
    }

    virtual std::string getCurrentIdentifier()
    {
        return fileName;
    }
public slots:
    virtual void executeAlgorithm(){}

    virtual void cancelAnalysis()
    {
        running = false;
    }
signals:
    void progress(int);
    void executionEnded(FA*);
protected:
    std::string fileName = "";
    std::string fileName2 = "";
    std::vector<double> ts;
    int tsLen;
    int N;
    std::vector<double> t = std::vector<double>();
    std::vector<double> y = std::vector<double>();
    std::vector<double> F = std::vector<double>();
    int minWin = 0;
    int maxWin = 0;
    double H = 0.0;
    double Hintercept = 0.0;

    inline static bool running = false;
};

#endif

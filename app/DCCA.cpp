#include "DCCA.h"

DCCA::DCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, std::string isAbs, int winStep, bool showProgBar)
    : FA(ts, tsLen)
{
    this->fileName = fileName;
    this->fileName2 = fileName2;
    this->tsLen2 = tsLen2;
    this->ts2.reserve(tsLen2);
    for(int i = 0; i < tsLen2; i++)
        this->ts2.push_back(ts2.at(i));
    this->minWin = minWin;
    this->maxWin = maxWin;
    this->ord = ord;
    this->isAbs = isAbs;
    this->winStep = winStep;
    this->showProgBar = showProgBar;
    getEqualLength();
}

DCCA::~DCCA(){}

QString DCCA::getAlgorithmStr()
{
    return strDCCA;
}

void DCCA::getEqualLength()
{
    int N1 = setTsLength(ts, tsLen);
    int N2 = setTsLength(ts2, tsLen2);
    if(N1 > N2)
        N = N2;
    else
		N = N1;
}

int DCCA::getTsLength()
{
	return N;
}

void DCCA::setVectors()
{
    FA::setVectors();
    MathOps mo;
    ArrayOps ao;
    std::vector<double> pn2 = std::vector<double>();
    std::vector<double> pn2Nomean = std::vector<double>();
    std::vector<double> pn2NoNan = std::vector<double>();
    ao.noNan(ts2, tsLen2, pn2NoNan);
    ao.sliceVec(pn2NoNan, pn2, 0, N - 1);
    mo.subtractMean(pn2, N, pn2Nomean);
    mo.cumsum(pn2Nomean, y2, N);
}
    
void DCCA::executeAlgorithm()
{
    running = true;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(minWin, maxWin, winStep);
    ao.intRange(s, range, minWin, winStep);

    for(int i = 0; i < range; i++)
    {
        QApplication::processEvents();
        if(!running)
            break;

        if(showProgBar)
            emit progress(i);
        else
            emit progressSingle(i);

        std::vector<double> Fnu = std::vector<double>();
        int currWinSize = s.at(i);
        int Ns = N - currWinSize;

        for(int v = 0; v < Ns; v++)
        {
            int startLim = v;
            int endLim = v + currWinSize;
            std::vector<double> tFit = std::vector<double>();
            std::vector<double> yFit1 = std::vector<double>();
            std::vector<double> yFit2 = std::vector<double>();
            std::vector<double> diffVec = std::vector<double>();
            std::vector<double> coeffs1 = std::vector<double>();
            std::vector<double> coeffs2 = std::vector<double>();
            ao.sliceVec(t, tFit, startLim, endLim);
            ao.sliceVec(y, yFit1, startLim, endLim);
            ao.sliceVec(y2, yFit2, startLim, endLim);
            mo.polyFit(currWinSize + 1, ord + 1, tFit, yFit1, coeffs1);
            mo.polyFit(currWinSize + 1, ord + 1, tFit, yFit2, coeffs2);
            if(isAbs.compare(corrDCCA.toStdString()) == 0)
            {
                for(int j = 0; j <= currWinSize; j++)
                {
                    double polySum1 = 0, polySum2 = 0;
                    for(int k = 0; k < ord+1; k++)
                    {
                        polySum1 += coeffs1.at(k) * pow(tFit.at(j), k);
                        polySum2 += coeffs2.at(k) * pow(tFit.at(j), k);
                    }
                    diffVec.push_back((yFit1.at(j) - polySum1) * (yFit2.at(j) - polySum2));
                }
            }
            else if(isAbs.compare(defaultDCCA.toStdString()) == 0)
            {
                for(int j = 0; j <= currWinSize; j++)
                {
                    double polySum1 = 0, polySum2 = 0;
                    for(int k = 0; k < ord + 1; k++)
                    {
                        polySum1 += coeffs1.at(k) * pow(tFit.at(j), k);
                        polySum2 += coeffs2.at(k) * pow(tFit.at(j), k);
                    }
                    diffVec.push_back(fabs((yFit1.at(j) - polySum1) * (yFit2.at(j) - polySum2)));
                }
            }
            Fnu.push_back(mo.customMean(diffVec, currWinSize + 1, currWinSize - 1));
        }
        if(isAbs.compare(corrDCCA.toStdString()) == 0)
            F.push_back(mo.mean(Fnu, Ns));
        else if(isAbs.compare(defaultDCCA.toStdString()) == 0)
            F.push_back(sqrt(mo.mean(Fnu, Ns)));
    }

    if(showProgBar && running)
    {
        emit progress(range);
        emit executionEnded(this);
    }
}

std::string DCCA::getFileName()
{
    return fileName;
}

std::string DCCA::getFileName2()
{
    return fileName2;
}

std::vector<double> DCCA::getF()
{
    return F;
}

int DCCA::getMinWin()
{
    return minWin;
}

int DCCA::getMaxWin()
{
    return maxWin;
}

double DCCA::getH()
{
	return H;
}

double DCCA::getHintercept()
{
	return Hintercept;
}

void DCCA::executeFit(int start, int end)
{
    MathOps mo = MathOps();
	int range = getRangeLength(start, end, winStep);
    std::vector<double> logS = std::vector<double>();
    std::vector<double> logF = std::vector<double>();
    for(int i = (start - minWin) / winStep; i <= (end - minWin) / winStep; i++)
    {
        logS.push_back(log(s.at(i)));
        logF.push_back(log(F.at(i)));
    }
    mo.linFit(range, logS, logF, &H, &Hintercept);
}

bool DCCA::isFittable()
{
    return true;
}

int DCCA::getLogType()
{
    return BasePlot::XY;
}

std::string DCCA::outFileStr()
{
    size_t a = fileName.find_last_of("/");
    size_t b = fileName.find_last_of(".");
    return "/" + DCCAfnStart + "_" + std::to_string(minWin) + "_" +
            std::to_string(maxWin) + "_" +
            fileName.substr(a + 1, b - a - 1) + "_" +
            fileName2.substr(fileName2.find_last_of("/") + 1);
}

void DCCA::saveFile(std::string pathTot)
{
    FileOps fo = FileOps();
	int range = getRangeLength(minWin, maxWin, winStep);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
    fprintf(f, "# scale F\n");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s.at(i), F.at(i));
    fclose(f);
}

void DCCA::plot(BasePlot *plt)
{
    int len = getRangeLength(minWin, maxWin, winStep);
    QVector<double> pltVec(len), n(len), Hfit(len);
    for(int i = 0; i < len; i++)
    {
        n[i] = log(s.at(i));
        pltVec[i] = log(F.at(i));
        Hfit[i] = Hintercept + H * n[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("log[n]");
    plt->yAxis->setLabel("log[F(n)]");
    plt->graph(0)->setData(n, pltVec);
    plt->graph(0)->setLineStyle(QCPGraph::lsNone);
    plt->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::red, 10));
    QString fn = QString::fromStdString(fileName).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    QString fn2 = QString::fromStdString(fileName2).split("/").last();
    fn2.truncate(fn2.lastIndexOf("."));
    plt->graph(0)->setName(fn + "_" + fn2 + "_" + QString::number(minWin) + "_" + QString::number(maxWin));
    plt->graph(0)->rescaleAxes();
    plt->addGraph();
    plt->graph(1)->setData(n, Hfit);
    QPen pen;
    pen.setWidth(2);
    plt->graph(1)->setPen(pen);
    plt->graph(1)->setName("H = " + QString::number(H));
    plt->graph(1)->rescaleAxes(true);
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

int DCCA::getAlgorithmTotalSteps()
{
    return getRangeLength(minWin, maxWin, winStep);
}

std::string DCCA::getCurrentIdentifier()
{
    return fileName.substr(fileName.find_last_of("/") + 1) +
           "\nVS\n" +
           fileName2.substr(fileName2.find_last_of("/") + 1);
}

#include "DFA.h"

DFA::DFA(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, int winStep, int revSeg)
    : FA(ts, tsLen)
{
    this->fileName = fileName;
    this->minWin = minWin;
    this->maxWin = maxWin;
    this->ord = ord;
    this->revSeg = revSeg;
    this->winStep = winStep;
    N = setTsLength(ts, tsLen);
}

DFA::~DFA(){}

QString DFA::getAlgorithmStr()
{
    return strDFA;
}

int DFA::getTsLength()
{
	return N;
}
    
void DFA::executeAlgorithm()
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

        emit progress(i);

        std::vector<double> Fnu1 = std::vector<double>();
        std::vector<double> Fnu2 = std::vector<double>();
        int currWinSize = s.at(i);
        int Ns = N / currWinSize;
        for(int v = 0; v < Ns; v++)
        {
            int startLim = v * currWinSize;
            int endLim = (v + 1) * currWinSize - 1;
            std::vector<double> tFit = std::vector<double>();
            std::vector<double> yFit = std::vector<double>();
            std::vector<double> diffVec = std::vector<double>();
            std::vector<double> coeffs = std::vector<double>();
            ao.zeroVec(diffVec, currWinSize);
            ao.sliceVec(t, tFit, startLim, endLim);
            ao.sliceVec(y, yFit, startLim, endLim);
            mo.polyFit(currWinSize, ord + 1, tFit, yFit, coeffs);
            for(int j = 0; j < currWinSize; j++)
            {
                for(int k = 0; k < ord + 1; k++)
                    diffVec.at(j) += coeffs.at(k) * pow(tFit.at(j), k);
                diffVec.at(j) = pow(yFit.at(j) - diffVec.at(j), 2.0);
            }
            Fnu1.push_back(mo.mean(diffVec, currWinSize));
        }

        if(revSeg == 1)
        {
            for(int v = 0; v < Ns; v++)
            {
                int startLim = v * currWinSize + (N - Ns * currWinSize);
                int endLim = (v + 1) * currWinSize + (N - Ns * currWinSize) - 1;
                std::vector<double> tFit = std::vector<double>();
                std::vector<double> yFit = std::vector<double>();
                std::vector<double> diffVec = std::vector<double>();
                std::vector<double> coeffs = std::vector<double>();
                ao.zeroVec(diffVec, currWinSize);
                ao.sliceVec(t, tFit, startLim, endLim);
                ao.sliceVec(y, yFit, startLim, endLim);
                mo.polyFit(currWinSize, ord + 1, tFit, yFit, coeffs);
                for(int j = 0; j < currWinSize; j++)
                {
                    for(int k = 0; k < ord + 1; k++)
                        diffVec.at(j) += coeffs.at(k) * pow(tFit.at(j), k);
                    diffVec.at(j) = pow(yFit.at(j) - diffVec.at(j), 2.0);
                }
                Fnu2.push_back(mo.mean(diffVec, currWinSize));
            }
            F.push_back(sqrt((mo.mean(Fnu1, Ns) + mo.mean(Fnu2, Ns)) / 2.0));
        }
        else
        {
            F.push_back(sqrt(mo.mean(Fnu1, Ns)));
        }
    }

    if(running)
    {
        emit progress(range);
        emit executionEnded(this);
    }
}

std::string DFA::getFileName()
{
    return fileName;
}

int DFA::getMinWin()
{
    return minWin;
}

int DFA::getMaxWin()
{
    return maxWin;
}

double DFA::getH()
{
	return H;
}

double DFA::getHintercept()
{
	return Hintercept;
}

void DFA::executeFit(int start, int end)
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

bool DFA::isFittable()
{
    return true;
}

int DFA::getLogType()
{
    return BasePlot::XY;
}

std::string DFA::outFileStr()
{
    return "/" + DFAfnStart + "_" + std::to_string(minWin) + "_" +
            std::to_string(maxWin) + "_" +
            fileName.substr(fileName.find_last_of("/") + 1);
}

void DFA::saveFile(std::string pathTot)
{
    FileOps fo = FileOps();
	int range = getRangeLength(minWin, maxWin, winStep);
	FILE *f;
    f = fo.openFile(pathTot + outFileStr(), "w");
    fprintf(f, "# scale F\n");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s.at(i), F.at(i));
    fclose(f);
}

void DFA::plot(BasePlot *plt)
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
    plt->graph(0)->setName(fn + "_" + QString::number(minWin) + "_" + QString::number(maxWin));
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

int DFA::getAlgorithmTotalSteps()
{
    return getRangeLength(minWin, maxWin, winStep);
}

std::string DFA::getCurrentIdentifier()
{
    return fileName.substr(fileName.find_last_of("/") + 1);
}

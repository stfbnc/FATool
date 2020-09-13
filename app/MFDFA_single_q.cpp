#include "MFDFA_single_q.h"

MFDFAsingleQ::MFDFAsingleQ(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, double q, int winStep, int revSeg) :
    FA(ts, tsLen)
{
    this->fileName = fileName;
    this->minWin = minWin;
    this->maxWin = maxWin;
    this->ord = ord;
    this->q = q;
    this->winStep = winStep;
    this->revSeg = revSeg;
    N = setTsLength(ts, tsLen);
}

MFDFAsingleQ::~MFDFAsingleQ(){}

QString MFDFAsingleQ::getAlgorithmStr()
{
    return strMFDFA;
}

int MFDFAsingleQ::getTsLength()
{
	return N;
}
    
bool MFDFAsingleQ::executeAlgorithm(){
    F.clear();
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(minWin, maxWin, winStep);
    ao.intRange(s, range, minWin, winStep);

    QProgressDialog progress(strMFDFA+"\n"+"q = "+QString::number(q)+" -> "+
                             QString::fromStdString(fileName.substr(fileName.find_last_of("/")+1)),
                             "Stop", 0, range);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setFixedSize(xPG, yPG);

    for(int i = 0; i < range; i++)
    {
        progress.setValue(i);
        if(progress.wasCanceled())
        {
            execStop = true;
            break;
        }

        std::vector<double> Fnu1 = std::vector<double>();
        std::vector<double> Fnu2 = std::vector<double>();
        int currWinSize = s.at(i);
        int Ns = N / currWinSize;
        for(int v = 0; v < Ns; v++){
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
                diffVec.at(j) = pow((yFit.at(j) - diffVec.at(j)), 2.0);
            }

            if(q == 0.0)
                Fnu1.push_back(log(mo.mean(diffVec, currWinSize)));
            else
                Fnu1.push_back(pow(mo.mean(diffVec, currWinSize), 0.5 * q));
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
                    for(int k = 0; k < ord+1; k++)
                        diffVec.at(j) += coeffs.at(k) * pow(tFit.at(j), k);
                    diffVec.at(j) = pow((yFit.at(j) - diffVec.at(j)), 2.0);
                }
                if(q == 0.0)
                    Fnu2.push_back(log(mo.mean(diffVec, currWinSize)));
                else
                    Fnu2.push_back(pow(mo.mean(diffVec, currWinSize), 0.5 * q));
            }

            if(q == 0.0)
                F.push_back(exp((mo.customMean(Fnu1, Ns, 2 * Ns) + mo.customMean(Fnu2, Ns, 2*Ns)) / 2.0));
            else
                F.push_back(pow((mo.mean(Fnu1, Ns) + mo.mean(Fnu2, Ns)) / 2.0, 1/static_cast<double>(q)));
        }
        else
        {
            if(q == 0.0)
                F.push_back(exp(mo.customMean(Fnu1, Ns, 2 * Ns)));
            else
                F.push_back(pow(mo.mean(Fnu1, Ns), 1/static_cast<double>(q)));
        }
    }

    progress.setValue(range);

    return execStop;
}

std::string MFDFAsingleQ::getFileName()
{
    return fileName;
}

int MFDFAsingleQ::getMinWin()
{
    return minWin;
}

int MFDFAsingleQ::getMaxWin()
{
    return maxWin;
}

double MFDFAsingleQ::getH()
{
	return H;
}

double MFDFAsingleQ::getHintercept()
{
	return Hintercept;
}

std::vector<double> MFDFAsingleQ::getF()
{
    return F;
}

void MFDFAsingleQ::executeFit(int start, int end)
{
    MathOps mo = MathOps();
	int range = getRangeLength(start, end, winStep);
    std::vector<double> logS = std::vector<double>();
    std::vector<double> logF = std::vector<double>();
    for(int i = (start-minWin)/winStep; i <= (end-minWin)/winStep; i++)
    {
        logS.push_back(log(s.at(i)));
        logF.push_back(log(F.at(i)));
    }
    mo.linFit(range, logS, logF, &H, &Hintercept);
}

bool MFDFAsingleQ::isFittable()
{
    return false;
}

int MFDFAsingleQ::getLogType()
{
    return BasePlot::NO_AX;
}

std::string MFDFAsingleQ::outFileStr()
{
    return "/" + MFDFAsQfnStart + "_" + std::to_string(minWin) + "_" +
            std::to_string(maxWin) + "_q" + std::to_string(static_cast<int>(q)) +
            "_" + fileName.substr(fileName.find_last_of("/") + 1);
}

void MFDFAsingleQ::saveFile(std::string pathTot)
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

void MFDFAsingleQ::plot(BasePlot *plt)
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
    plt->graph(0)->setName(fn+"_"+QString::number(minWin)+"_"+QString::number(maxWin)+"_q"+QString::number(q));
    plt->graph(0)->rescaleAxes();
    plt->addGraph();
    plt->graph(1)->setData(n, Hfit);
    QPen pen;
    pen.setWidth(2);
    plt->graph(1)->setPen(pen);
    plt->graph(1)->setName("H = "+QString::number(H));
    plt->graph(1)->rescaleAxes(true);
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

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
    allocateMemory();
}

DFA::~DFA(){}

void DFA::allocateMemory()
{
    t.reserve(N);
    y.reserve(N);
    s.reserve(getRangeLength(minWin, maxWin, winStep));
    F.reserve(getRangeLength(minWin, maxWin, winStep));
}

int DFA::getTsLength()
{
	return N;
}
    
bool DFA::computeFlucVec()
{
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(minWin, maxWin, winStep);
    ao.intRange(s, range, minWin, winStep);
	int Flen = N / minWin;
    std::vector<double> Fnu1, Fnu2;
    Fnu1.reserve(Flen);
    Fnu2.reserve(Flen);

    QProgressDialog progress(strDFA+"\n"+QString::fromStdString(fileName.substr(fileName.find_last_of("/")+1)), "Stop", 0, range);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setFixedSize(xPG, yPG);

    for(int i = 0; i < range; i++){
        progress.setValue(i);
        if(progress.wasCanceled()){
            execStop = true;
            break;
        }
        int currWinSize = s.at(i);
        int Ns = N / currWinSize;
        ao.zeroVec(Fnu1, Flen);
        #pragma omp parallel for
        for(int v = 0; v < Ns; v++){
            int startLim = v * currWinSize;
            int endLim = (v + 1) * currWinSize - 1;
            std::vector<double> tFit, yFit, diffVec, coeffs;
            tFit.reserve(currWinSize);
            yFit.reserve(currWinSize);
            diffVec.reserve(currWinSize);
            coeffs.reserve(ord+1);
            ao.zeroVec(diffVec, currWinSize);
            ao.sliceVec(t, tFit, startLim, endLim);
            ao.sliceVec(y, yFit, startLim, endLim);
            mo.polyFit(currWinSize, ord+1, tFit, yFit, coeffs);
            for(int j = 0; j < currWinSize; j++){
                for(int k = 0; k < ord+1; k++)
                    diffVec.at(j) += coeffs.at(k) * pow(tFit.at(j), k);
                diffVec.at(j) = pow(yFit.at(j) - diffVec.at(j), 2.0);
            }
            Fnu1.at(v) = mo.mean(diffVec, currWinSize);
        }
        if(revSeg == 1){
            ao.zeroVec(Fnu2, Flen);
            #pragma omp parallel for
            for(int v = 0; v < Ns; v++){
                int startLim = v * currWinSize + (N - Ns * currWinSize);
                int endLim = (v + 1) * currWinSize + (N - Ns * currWinSize) - 1;
                std::vector<double> tFit, yFit, diffVec, coeffs;
                tFit.reserve(currWinSize);
                yFit.reserve(currWinSize);
                diffVec.reserve(currWinSize);
                coeffs.reserve(ord+1);
                ao.zeroVec(diffVec, currWinSize);
                ao.sliceVec(t, tFit, startLim, endLim);
                ao.sliceVec(y, yFit, startLim, endLim);
                mo.polyFit(currWinSize, ord+1, tFit, yFit, coeffs);
                for(int j = 0; j < currWinSize; j++){
                    for(int k = 0; k < ord+1; k++)
                        diffVec.at(j) += coeffs.at(k) * pow(tFit.at(j), k);
                    diffVec.at(j) = pow(yFit.at(j) - diffVec.at(j), 2.0);
                }
                Fnu2.at(v) = mo.mean(diffVec, currWinSize);
            }
            F.at(i) = sqrt((mo.mean(Fnu1, Ns) + mo.mean(Fnu2, Ns)) / 2.0);
        }else{
            F.at(i) = sqrt(mo.mean(Fnu1, Ns));
        }
    }

    progress.setValue(range);

    return execStop;
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

int DFA::getWinStep()
{
    return winStep;
}

double DFA::getH()
{
	return H;
}

double DFA::getHintercept()
{
	return Hintercept;
}

void DFA::fitFlucVec(int start, int end)
{
    MathOps mo = MathOps();
	int range = getRangeLength(start, end, winStep);
    std::vector<double> logS, logF;
    logS.reserve(range);
    logF.reserve(range);
    int idx = 0;
    for(int i = (start-minWin)/winStep; i <= (end-minWin)/winStep; i++){
        logS.at(idx) = log(s.at(i));
        logF.at(idx) = log(F.at(i));
        idx++;
    }
    mo.linFit(range, logS, logF, &H, &Hintercept);
}

std::string DFA::outFileStr()
{
    return "/"+DFAfnStart+"_"+std::to_string(minWin)+"_"+std::to_string(maxWin)+"_"+fileName.substr(fileName.find_last_of("/")+1);
}

void DFA::saveFile(std::string pathTot)
{
    FileOps fo = FileOps();
	int range = getRangeLength(minWin, maxWin, winStep);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

void DFA::plot(BasePlot *plt)
{
    int len = getRangeLength(minWin, maxWin, winStep);
    QVector<double> pltVec(len), n(len), Hfit(len);
    for(int i = 0; i < len; i++){
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
    plt->graph(0)->setName(fn+"_"+QString::number(minWin)+"_"+QString::number(maxWin));
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

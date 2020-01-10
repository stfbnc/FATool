#include "MFDFA_single_q.h"

MFDFAsingleQ::MFDFAsingleQ(std::string fileName_, int minWin_, int maxWin_, int ord_, double q_, int winStep_, int revSeg_)
	: FA()
{
	fileName = fileName_;
	minWin = minWin_;
	maxWin = maxWin_;
	ord = ord_;
	q = q_;
	winStep = winStep_;
	revSeg = revSeg_;
    checkFileExistence(fileName);
	N = setTsLength(fileName);
    allocateMemory();
}

MFDFAsingleQ::~MFDFAsingleQ(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<int>(s);
	delAlloc<double>(F);
}

void MFDFAsingleQ::allocateMemory(){
	t = new double [N];
	y = new double [N];
	s = new int [getRangeLength(minWin, maxWin, winStep)];
	F = new double [getRangeLength(minWin, maxWin, winStep)];
}

int MFDFAsingleQ::getTsLength(){
	return N;
}
    
bool MFDFAsingleQ::computeFlucVec(){
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(minWin, maxWin, winStep);
    ao.intRange(s, range, minWin, winStep);
	int Flen = N / minWin;
    double *Fnu1, *Fnu2;
    Fnu1 = new double [Flen];
    Fnu2 = new double [Flen];

    QProgressDialog progress(strMFDFA+"\n"+"q = "+QString::number(q)+" -> "+
                             QString::fromStdString(fileName.substr(fileName.find_last_of("/")+1)),
                             "Stop", 0, range);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setFixedSize(xPG, yPG);

    for(int i = 0; i < range; i++){
        progress.setValue(i);
        if(progress.wasCanceled()){
            execStop = true;
            break;
        }
        int currWinSize = s[i];
        int Ns = N / currWinSize;
        ao.zeroVec(Fnu1, Flen);
        #pragma omp parallel for
        for(int v = 0; v < Ns; v++){
            int startLim = v * currWinSize;
            int endLim = (v + 1) * currWinSize - 1;
            double *tFit, *yFit, *diffVec, *coeffs;
            tFit = new double [currWinSize];
            yFit = new double [currWinSize];
            diffVec = new double [currWinSize];
            coeffs = new double [ord+1];
            ao.zeroVec(diffVec, currWinSize);
            ao.sliceVec(t, tFit, startLim, endLim);
            ao.sliceVec(y, yFit, startLim, endLim);
            mo.polyFit(currWinSize, ord+1, tFit, yFit, coeffs);
            for(int j = 0; j < currWinSize; j++){
                for(int k = 0; k < ord+1; k++)
                    diffVec[j] += coeffs[k] * pow(tFit[j], k);
                diffVec[j] = pow((yFit[j] - diffVec[j]), 2.0);
            }
            if(q == 0.0){
                Fnu1[v] = log(mo.mean(diffVec, currWinSize));
            }else{
                Fnu1[v] = pow(mo.mean(diffVec, currWinSize), 0.5*q);
            }
            delAlloc<double>(tFit);
            delAlloc<double>(yFit);
            delAlloc<double>(diffVec);
            delAlloc<double>(coeffs);
        }
        if(revSeg == 1){
            ao.zeroVec(Fnu2, Flen);
            #pragma omp parallel for
            for(int v = 0; v < Ns; v++){
                int startLim = v * currWinSize + (N - Ns * currWinSize);
                int endLim = (v + 1) * currWinSize + (N - Ns * currWinSize) - 1;
                double *tFit, *yFit, *diffVec, *coeffs;
                tFit = new double [currWinSize];
                yFit = new double [currWinSize];
                diffVec = new double [currWinSize];
                coeffs = new double [ord+1];
                ao.zeroVec(diffVec, currWinSize);
                ao.sliceVec(t, tFit, startLim, endLim);
                ao.sliceVec(y, yFit, startLim, endLim);
                mo.polyFit(currWinSize, ord+1, tFit, yFit, coeffs);
                for(int j = 0; j < currWinSize; j++){
                    for(int k = 0; k < ord+1; k++)
                        diffVec[j] += coeffs[k] * pow(tFit[j], k);
                    diffVec[j] = pow((yFit[j] - diffVec[j]), 2.0);
                }
                if(q == 0.0){
                    Fnu2[v] = log(mo.mean(diffVec, currWinSize));
                }else{
                    Fnu2[v] = pow(mo.mean(diffVec, currWinSize), 0.5*q);
                }
                delAlloc<double>(tFit);
                delAlloc<double>(yFit);
                delAlloc<double>(diffVec);
                delAlloc<double>(coeffs);
            }
            if(q == 0.0){
                F[i] = exp((mo.customMean(Fnu1, Ns, 2*Ns) + mo.customMean(Fnu2, Ns, 2*Ns)) / 2.0);
            }else{
                F[i] = pow((mo.mean(Fnu1, Ns) + mo.mean(Fnu2, Ns)) / 2.0, 1/static_cast<double>(q));
            }
        }else{
            if(q == 0.0){
                F[i] = exp(mo.customMean(Fnu1, Ns, 2*Ns));
            }else{
                F[i] = pow(mo.mean(Fnu1, Ns), 1/static_cast<double>(q));
            }
        }
    }

    progress.setValue(range);
    delAlloc<double>(Fnu1);
    delAlloc<double>(Fnu2);

    return execStop;
}

std::string MFDFAsingleQ::getFileName(){
    return fileName;
}

double MFDFAsingleQ::getH(){
	return H;
}

double MFDFAsingleQ::getHintercept(){
	return Hintercept;
}

void MFDFAsingleQ::fitFlucVec(int start, int end){
    MathOps mo = MathOps();
	int range = getRangeLength(start, end, winStep);
    double *logS, *logF;
    logS = new double [range];
    logF = new double [range];
    int idx = 0;
    for(int i = (start-minWin)/winStep; i <= (end-minWin)/winStep; i++){
        logS[idx] = log(s[i]);
        logF[idx] = log(F[i]);
        idx++;
    }
    mo.linFit(range, logS, logF, &H, &Hintercept);
    delAlloc<double>(logS);
    delAlloc<double>(logF);
}

std::string MFDFAsingleQ::outFileStr(){
    return "/"+MFDFAsQfnStart+"_"+std::to_string(minWin)+"_"+std::to_string(maxWin)+"_q"+std::to_string(static_cast<int>(q))+"_"+
			fileName.substr(fileName.find_last_of("/")+1);
}

void MFDFAsingleQ::saveFile(std::string pathTot){
    FileOps fo = FileOps();
	int range = getRangeLength(minWin, maxWin, winStep);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

void MFDFAsingleQ::plot(BasePlot *plt){
    int len = getRangeLength(minWin, maxWin, winStep);
    QVector<double> pltVec(len), n(len), Hfit(len);
    for(int i = 0; i < len; i++){
        n[i] = log(s[i]);
        pltVec[i] = log(F[i]);
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

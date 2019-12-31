#include "DCCA.h"

DCCA::DCCA(std::string fileName_, std::string fileName2_, int minWin_, int maxWin_, int ord_, std::string isAbs_, int winStep_)
	: FA()
{
	fileName = fileName_;
	fileName2 = fileName2_;
	minWin = minWin_;
	maxWin = maxWin_;
	ord = ord_;
	isAbs = isAbs_;
	winStep = winStep_;
    checkFileExistence(fileName);
    checkFileExistence(fileName2);
	getEqualLength(fileName, fileName2);
    allocateMemory();
}

DCCA::~DCCA(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<double>(y2);
	delAlloc<int>(s);
	delAlloc<double>(F);
}

void DCCA::allocateMemory(){
	t = new double [N];
	y = new double [N];
	y2 = new double [N];
	s = new int [getRangeLength(minWin, maxWin, winStep)];
	F = new double [getRangeLength(minWin, maxWin, winStep)];
}

void DCCA::getEqualLength(std::string fn1, std::string fn2){
	int N1 = setTsLength(fn1);
    int N2 = setTsLength(fn2);
    if(N1 > N2){
        N = N2;
    }else{
		N = N1;
	}
}

int DCCA::getTsLength(){
	return N;
}

void DCCA::setFlucVectors(){
	FA::setFlucVectors();
    MathOps mo = MathOps();
    FileOps fo = FileOps();
    double *pn2, *pn2Nomean;
    pn2 = new double [N];
    pn2Nomean = new double [N];
	FILE *f;
	f = fo.openFile(fileName2, "r");
	for(int i = 0; i < N; i++)
		fscanf(f, "%lf", &pn2[i]);
	fclose(f);
	mo.subtractMean(pn2, N, pn2Nomean);
	mo.cumsum(pn2Nomean, y2, N);
    delAlloc<double>(pn2);
    delAlloc<double>(pn2Nomean);
}
    
bool DCCA::computeFlucVec(){
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(minWin, maxWin, winStep);
    ao.intRange(s, range, minWin, winStep);
	int Flen = N - minWin;
    double *Fnu;
    Fnu = new double [Flen];

    QProgressDialog progress(strDCCA+"\n"+QString::fromStdString(fileName.substr(fileName.find_last_of("/")+1))+
                             " vs "+QString::fromStdString(fileName2.substr(fileName2.find_last_of("/")+1)), "Stop", 0, range);
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
        int Ns = N - currWinSize;
        ao.zeroVec(Fnu, Flen);

        #pragma omp parallel for
        for(int v = 0; v < Ns; v++){
            int startLim = v;
            int endLim = v + currWinSize;
            double *tFit, *yFit1, *yFit2, *diffVec;
            tFit = new double [currWinSize+1];
            yFit1 = new double [currWinSize+1];
            yFit2 = new double [currWinSize+1];
            diffVec = new double [currWinSize+1];
            ao.sliceVec(t, tFit, startLim, endLim);
            ao.sliceVec(y, yFit1, startLim, endLim);
            ao.sliceVec(y2, yFit2, startLim, endLim);
            double angCoeff1, intercept1, angCoeff2, intercept2;
            mo.linFit(currWinSize+1, tFit, yFit1, &angCoeff1, &intercept1);
            mo.linFit(currWinSize+1, tFit, yFit2, &angCoeff2, &intercept2);
            if(isAbs.compare(corrDCCA) == 0){
                for(int j = 0; j <= currWinSize; j++)
                    diffVec[j] = (yFit1[j] - (intercept1 + angCoeff1 * tFit[j])) * (yFit2[j] - (intercept2 + angCoeff2 * tFit[j]));
            }else if(isAbs.compare(defaultDCCA) == 0){
                for(int j = 0; j <= currWinSize; j++)
                    diffVec[j] = fabs((yFit1[j] - (intercept1 + angCoeff1 * tFit[j])) * (yFit2[j] - (intercept2 + angCoeff2 * tFit[j])));
            }
            Fnu[v] = mo.customMean(diffVec, currWinSize+1, currWinSize-1);
            delAlloc<double>(tFit);
            delAlloc<double>(yFit1);
            delAlloc<double>(yFit2);
            delAlloc<double>(diffVec);
        }
        if(isAbs.compare(corrDCCA) == 0){
            F[i] = mo.mean(Fnu, Ns);
        }else if(isAbs.compare(defaultDCCA) == 0){
            F[i] = sqrt(mo.mean(Fnu, Ns));
        }
    }

    progress.setValue(range);
    delAlloc<double>(Fnu);

    return execStop;
}

std::string DCCA::getFileName1(){
    return fileName;
}

std::string DCCA::getFileName2(){
    return fileName2;
}

double* DCCA::getF(){
    return F;
}

int DCCA::getMinWin(){
    return minWin;
}

int DCCA::getMaxWin(){
    return maxWin;
}

double DCCA::getH(){
	return H;
}

double DCCA::getHintercept(){
	return Hintercept;
}

void DCCA::fitFlucVec(int start, int end){
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

std::string DCCA::outFileStr(){
    size_t a = fileName.find_last_of("/");
    size_t b = fileName.find_last_of(".");
    return "/"+DCCAfnStart+"_"+std::to_string(minWin)+"_"+std::to_string(maxWin)+"_"
            +fileName.substr(a+1, b-a-1)+"_"
			+fileName2.substr(fileName2.find_last_of("/")+1);
}

void DCCA::saveFile(std::string pathTot){
    FileOps fo = FileOps();
	int range = getRangeLength(minWin, maxWin, winStep);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

void DCCA::plot(BasePlot *plt){
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
    QString fn2 = QString::fromStdString(fileName2).split("/").last();
    fn2.truncate(fn2.lastIndexOf("."));
    plt->graph(0)->setName(fn+"_"+fn2+"_"+QString::number(minWin)+"_"+QString::number(maxWin));
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

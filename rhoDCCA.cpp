#include "rhoDCCA.h"

rhoDCCA::rhoDCCA(std::string fileName_, double *ts_, int tsLen_, std::string fileName2_, double *ts2_, int tsLen2_, int minWin_, int maxWin_, int ord_, int winStep_, bool thresh_){
	fileName = fileName_;
    ts = ts_;
    tsLen = tsLen_;
	fileName2 = fileName2_;
    ts2 = ts2_;
    tsLen2 = tsLen2_;
	minWin = minWin_;
	maxWin = maxWin_;
	ord = ord_;
	winStep = winStep_;
    thresh = thresh_;
	L = 0;
    rho = nullptr;
}

rhoDCCA::~rhoDCCA() {
    delete [] ts;
    delete [] ts2;
    delete [] rho;
    if(thresh){
        delete [] confDown;
        delete [] confUp;
    }else{
        delete confDown;
        delete confUp;
    }
}

bool rhoDCCA::computeRho(){
    bool execStop = false;
    DCCA dccaXX = DCCA(fileName, ts, tsLen, fileName, ts, tsLen, minWin, maxWin, ord, defaultDCCA, winStep);
    dccaXX.setFlucVectors();
    execStop = dccaXX.computeFlucVec();
    if(!execStop){
        double *Fxx = dccaXX.getF();
        DCCA dccaYY = DCCA(fileName2, ts2, tsLen2, fileName2, ts2, tsLen2, minWin, maxWin, ord, defaultDCCA, winStep);
        dccaYY.setFlucVectors();
        execStop = dccaYY.computeFlucVec();
        if(!execStop){
            double *Fyy = dccaYY.getF();
            DCCA dccaXY = DCCA(fileName, ts, tsLen, fileName2, ts2, tsLen2, minWin, maxWin, ord, corrDCCA, winStep);
            dccaXY.setFlucVectors();
            execStop = dccaXY.computeFlucVec();
            if(!execStop){
                double *Fxy = dccaXY.getF();
                L = dccaXY.getRangeLength(minWin, maxWin, winStep);
                N = dccaXY.getTsLength();
                rho = new double [L];
                for(int i = 0; i < L; i++)
                    rho[i] = Fxy[i] / static_cast<double>(Fxx[i] * Fyy[i]);
            }
        }
    }
    return execStop;
}

void rhoDCCA::computeThresholds(){
    bool execStop;
    MathOps mo;
    double *grand1, *grand2;
    grand1 = new double [N];
    grand2 = new double [N];
    double *rhoMtx;
    rhoMtx = new double [getRhoLength()*nSim];
    for(int i = 0; i < nSim; i++){
        mo.gaussRand(grand1, N);
        mo.gaussRand(grand2, N);
        std::string s1 = "rand1 (sim "+std::to_string(i+1)+")";
        std::string s2 = "rand2 (sim "+std::to_string(i+1)+")";
        DCCA dcca11 = DCCA(s1, grand1, N, s1, grand1, N, minWin, maxWin, ord, defaultDCCA, winStep);
        dcca11.setFlucVectors();
        execStop = dcca11.computeFlucVec();
        if(!execStop){
            double *F11 = dcca11.getF();
            DCCA dcca22 = DCCA(s2, grand2, N, s2, grand2, N, minWin, maxWin, ord, defaultDCCA, winStep);
            dcca22.setFlucVectors();
            execStop = dcca22.computeFlucVec();
            if(!execStop){
                double *F22 = dcca22.getF();
                DCCA dcca12 = DCCA(s1, grand1, N, s2, grand2, N, minWin, maxWin, ord, corrDCCA, winStep);
                dcca12.setFlucVectors();
                execStop = dcca12.computeFlucVec();
                if(!execStop){
                    double *F12 = dcca12.getF();
                    for(int j = 0; j < getRhoLength(); j++)
                        rhoMtx[i*nSim+j] = F12[j] / static_cast<double>(F11[j] * F22[j]);
                }else{
                    thresh = false;
                    break;
                }
            }else{
                thresh = false;
                break;
            }
        }else{
            thresh = false;
            break;
        }
    }
    if(!execStop)
        confLevHist(rhoMtx);
    delete [] grand1;
    delete [] grand2;
    delete [] rhoMtx;
}

void rhoDCCA::confLevHist(double *rhos){
    MathOps mo;
    ArrayOps ao;
    double *singleCol;
    singleCol = new double [getRhoLength()];
    confDown = new double [getRhoLength()];
    confUp = new double [getRhoLength()];
    int numBins = static_cast<int>(2.0 * pow(getRhoLength(), 1.0/3.0));
    for(int i = 0; i < getRhoLength(); i++){
        ao.extractColumn(rhos, getRhoLength(), nSim, i, singleCol);
        double minVal = mo.vecMin<double>(singleCol, getRhoLength());
        double maxVal = mo.vecMax<double>(singleCol, getRhoLength());
        double binWidth = (maxVal - minVal) / static_cast<double>(numBins);
        double *normHist;
        normHist = new double [numBins];
        ao.zeroVec(normHist, numBins);
        for(int j = 0; j < getRhoLength(); j++){
            int bin = floor((singleCol[i] - minVal) / binWidth);
            normHist[bin] += 1;
        }
        for(int j = 0; j < numBins; j++)
            normHist[j] /= static_cast<double>(binWidth * getRhoLength());
        double sumArea = 0.0, leftVal, rightVal;
        int idx = 0;
        while(sumArea <= confLev && idx < (numBins / 2)){
            leftVal = normHist[idx];
            rightVal = normHist[numBins-1-idx];
            sumArea += ((leftVal + rightVal) * binWidth);
            idx++;
        }
        idx++;
        confDown[i] = minVal + (idx * binWidth);
        confUp[i] = maxVal - (idx * binWidth);
        delete [] normHist;
    }
    delete [] singleCol;
}

int rhoDCCA::getRhoLength(){
    return L;
}

bool rhoDCCA::threshCompute(){
    return thresh;
}

std::string rhoDCCA::getFileName1(){
    return fileName;
}

std::string rhoDCCA::getFileName2(){
    return fileName2;
}

std::string rhoDCCA::outFileStr(){
    size_t a = fileName.find_last_of("/");
    size_t b = fileName.find_last_of(".");
    return "/"+RHODCCAfnStart+"_"+std::to_string(minWin)+"_"+std::to_string(maxWin)+"_"
            +fileName.substr(a+1, b-a-1)+"_"
			+fileName2.substr(fileName2.find_last_of("/")+1);
}

void rhoDCCA::saveFile(std::string pathTot){
    FileOps fo = FileOps();
    FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
    for(int i = 0; i < getRhoLength(); i++)
        fprintf(f, "%d %lf\n", (i*winStep)+minWin, rho[i]);
    fclose(f);
}

void rhoDCCA::plot(BasePlot *plt){
    QVector<double> n(L), corrs(L);
    for(int i = 0; i < L; i++){
        n[i] = (i * winStep) + minWin;
        corrs[i] = rho[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("n");
    plt->yAxis->setLabel("rho(n)");
    plt->graph(0)->setData(n, corrs);
    QPen pen;
    pen.setColor(Qt::red);
    plt->graph(0)->setPen(pen);
    plt->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
    QString fn = QString::fromStdString(fileName).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    QString fn2 = QString::fromStdString(fileName2).split("/").last();
    fn2.truncate(fn2.lastIndexOf("."));
    plt->graph(0)->setName(fn+"_"+fn2+"_"+QString::number(minWin)+"_"+QString::number(maxWin));
    plt->graph(0)->rescaleAxes();
    if(thresh){
        QVector<double> cd(L), cu(L);
        for(int i = 0; i < L; i++){
            cd[i] = confDown[i];
            cu[i] = confUp[i];
        }
        pen.setColor(Qt::black);
        pen.setWidth(2);
        plt->addGraph();
        plt->graph(1)->setData(n, cd);
        plt->graph(1)->setPen(pen);
        plt->graph(1)->setName("Lower "+QString::number(confLev*100.0)+"% CI");
        plt->graph(1)->rescaleAxes(true);
        plt->addGraph();
        plt->graph(2)->setData(n, cu);
        plt->graph(2)->setPen(pen);
        plt->graph(2)->setName("Upper "+QString::number(confLev*100.0)+"% CI");
        plt->graph(2)->rescaleAxes(true);
    }
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

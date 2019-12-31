#include "rhoDCCA.h"

rhoDCCA::rhoDCCA(std::string fileName_, std::string fileName2_, int minWin_, int maxWin_, int ord_, int winStep_){
	fileName = fileName_;
	fileName2 = fileName2_;
	minWin = minWin_;
	maxWin = maxWin_;
	ord = ord_;
	winStep = winStep_;
	L = 0;
    rho = nullptr;
}

rhoDCCA::~rhoDCCA() {
	delete [] rho;
}

bool rhoDCCA::computeRho(){
    bool execStop = false;
    DCCA dccaXX = DCCA(fileName, fileName, minWin, maxWin, ord, defaultDCCA, winStep);
    dccaXX.setFlucVectors();
    execStop = dccaXX.computeFlucVec();
    if(!execStop){
        double *Fxx = dccaXX.getF();
        DCCA dccaYY = DCCA(fileName2, fileName2, minWin, maxWin, ord, defaultDCCA, winStep);
        dccaYY.setFlucVectors();
        execStop = dccaYY.computeFlucVec();
        if(!execStop){
            double *Fyy = dccaYY.getF();
            DCCA dccaXY = DCCA(fileName, fileName2, minWin, maxWin, ord, corrDCCA, winStep);
            dccaXY.setFlucVectors();
            execStop = dccaXY.computeFlucVec();
            if(!execStop){
                double *Fxy = dccaXY.getF();
                L = dccaXY.getRangeLength(minWin, maxWin, winStep);
                rho = new double [L];
                for(int i = 0; i < L; i++)
                    rho[i] = Fxy[i] / static_cast<double>(Fxx[i] * Fyy[i]);
            }
        }
    }
    return execStop;
}

int rhoDCCA::getRhoLength(){
    return L;
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
    plt->yAxis->setLabel("rhoDCCA(n)");
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
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

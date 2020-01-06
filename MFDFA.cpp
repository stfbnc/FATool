#include "MFDFA.h"

MFDFA::MFDFA(std::string fileName_, int minWin_, int maxWin_, int ord_, double qIn_, int Nq_, int winStep_, double stepq_, int revSeg_)
	: MFDFAsingleQ(fileName_, minWin_, maxWin_, ord_, qIn_, winStep_, revSeg_)
{
	Nq = Nq_;
	stepq = stepq_;
    qRange = nullptr;
    flucMtx = nullptr;
    Hq = nullptr;
    Hinterceptq = nullptr;
    allocateQmemory();
}

MFDFA::~MFDFA(){
	delAlloc<double>(qRange);
	delAlloc<double>(Hq);
	delAlloc<double>(Hinterceptq);
	del2Alloc<double>(flucMtx, getRangeLength(minWin, maxWin, winStep));
}

void MFDFA::allocateQmemory(){
    qRange = new double [Nq];
    Hq = new double [Nq];
    Hinterceptq = new double [Nq];
    flucMtx = new double* [getRangeLength(minWin, maxWin, winStep)];
    for(int i = 0; i < getRangeLength(minWin, maxWin, winStep); i++){
    	flucMtx[i] = new double [Nq];
    }
}

void MFDFA::setQrange(double start, int len, double step){
    ArrayOps ao = ArrayOps();
    ao.doubleRange(qRange, len, start, step);
}

bool MFDFA::computeFlucVec(){
    bool execStop = false;
	setQrange(q, Nq, stepq);
	int Lq = getRangeLength(minWin, maxWin, winStep);
	for(int i = 0; i < Nq; i++){
		q = qRange[i];
        execStop = MFDFAsingleQ::computeFlucVec();
        if(!execStop){
            for(int j = 0; j < Lq; j++){
                flucMtx[j][i] = F[j];
            }
            fitFlucVec(minWin, maxWin);
            Hq[i] = getH();
            Hinterceptq[i] = getHintercept();
        }else{
            break;
        }
	}
    return execStop;
}

void MFDFA::computeMassExponents()
{
    tau = new double [Nq];
    for(int i = 0; i < Nq; i++)
        tau[i] = Hq[i] * qRange[i] - 1.0;
}

void MFDFA::computeSpectrum()
{
    computeMassExponents();
    alpha = new double [Nq-1];
    spectrum = new double [Nq-1];
    for(int i = 0; i < Nq-1; i++)
        alpha[i] = (tau[i+1] - tau[i]) / (qRange[1] - qRange[0]);
    for(int i = 0; i < Nq-1; i++)
        spectrum[i] = qRange[i] * alpha[i] - tau[i];
}

std::string MFDFA::outFileStr(){
    return "/"+MFDFAfnStart+"_"+std::to_string(minWin)+"_"+std::to_string(maxWin)+"_q"+std::to_string(static_cast<int>(qRange[0]))+"_"+
            std::to_string(static_cast<int>(qRange[Nq-1]))+"_"+fileName.substr(fileName.find_last_of("/")+1);
}

void MFDFA::saveFile(std::string pathTot){
	FileOps fo = FileOps();
	int Lq = getRangeLength(minWin, maxWin, winStep);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
	fprintf(f, "#q ");
	for(int i = 0; i < Nq; i++){
		i == Nq-1 ? fprintf(f, "%lf\n", qRange[i]) : fprintf(f, "%lf ", qRange[i]);
	}
    for(int i = 0; i < Lq; i++){
		fprintf(f, "%d ", s[i]);
		for(int j = 0; j < Nq; j++){
			j == Nq-1 ? fprintf(f, "%lf\n", flucMtx[i][j]) : fprintf(f, "%lf ", flucMtx[i][j]);
		}
	}
    fclose(f);
}

std::string MFDFA::qoutFileStr(){
    return "/"+MFDFAfnStart+"_q"+std::to_string(static_cast<int>(qRange[0]))+"_"+std::to_string(static_cast<int>(qRange[Nq-1]))+
			"_"+fileName.substr(fileName.find_last_of("/")+1);
}

void MFDFA::qsaveFile(std::string pathTot){
	FileOps fo = FileOps();
	FILE *f;
    f = fo.openFile(pathTot+qoutFileStr(), "w");
    for(int i = 0; i < Nq; i++){
		fprintf(f, "%lf %lf %lf\n", qRange[i], Hq[i], Hinterceptq[i]);
	}
    fclose(f);
}

std::string MFDFA::tauOutFileStr(){
    return "/tau_q"+std::to_string(static_cast<int>(qRange[0]))+"_"+std::to_string(static_cast<int>(qRange[Nq-1]))+
            "_"+fileName.substr(fileName.find_last_of("/")+1);
}

void MFDFA::tauSaveFile(std::string pathTot){
    FileOps fo = FileOps();
    FILE *f;
    f = fo.openFile(pathTot+tauOutFileStr(), "w");
    for(int i = 0; i < Nq; i++){
        fprintf(f, "%lf %lf\n", qRange[i], tau[i]);
    }
    fclose(f);
}

std::string MFDFA::spectrumOutFileStr(){
    return "/spectrum_q"+std::to_string(static_cast<int>(qRange[0]))+"_"+std::to_string(static_cast<int>(qRange[Nq-1]))+
            "_"+fileName.substr(fileName.find_last_of("/")+1);
}

void MFDFA::spectrumSaveFile(std::string pathTot){
    FileOps fo = FileOps();
    FILE *f;
    f = fo.openFile(pathTot+spectrumOutFileStr(), "w");
    for(int i = 0; i < Nq-1; i++){
        fprintf(f, "%lf %lf\n", alpha[i], spectrum[i]);
    }
    fclose(f);
}

void MFDFA::plot(BasePlot *plt){
    QVector<double> yh(Nq), xq(Nq);
    for(int i = 0; i < Nq; i++){
        xq[i] = qRange[i];
        yh[i] = Hq[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("q");
    plt->yAxis->setLabel("h(q)");
    plt->graph(0)->setData(xq, yh);
    plt->graph(0)->setLineStyle(QCPGraph::lsLine);
    plt->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::red, 10));
    QString fn = QString::fromStdString(fileName).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    plt->graph(0)->setName(fn);
    plt->graph(0)->rescaleAxes();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    plt->graph(0)->setPen(pen);
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

void MFDFA::plotMassExponents(BasePlot *plt)
{
    QVector<double> yt(Nq), xq(Nq);
    for(int i = 0; i < Nq; i++){
        xq[i] = qRange[i];
        yt[i] = tau[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("q");
    plt->yAxis->setLabel("tau(q)");
    plt->graph(0)->setData(xq, yt);
    plt->graph(0)->setLineStyle(QCPGraph::lsLine);
    plt->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::red, 10));
    QString fn = QString::fromStdString(fileName).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    plt->graph(0)->setName(fn);
    plt->graph(0)->rescaleAxes();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    plt->graph(0)->setPen(pen);
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

void MFDFA::plotSpectrum(BasePlot *plt)
{
    QVector<double> a(Nq-1), f(Nq-1);
    for(int i = 0; i < Nq-1; i++){
        a[i] = alpha[i];
        f[i] = spectrum[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("alpha");
    plt->yAxis->setLabel("f(alpha)");
    plt->graph(0)->setData(a, f);
    plt->graph(0)->setLineStyle(QCPGraph::lsLine);
    plt->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::red, 10));
    QString fn = QString::fromStdString(fileName).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    plt->graph(0)->setName(fn);
    plt->graph(0)->rescaleAxes();
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    plt->graph(0)->setPen(pen);
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

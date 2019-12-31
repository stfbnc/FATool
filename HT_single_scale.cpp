#include "HT_single_scale.h"

HTsingleScale::HTsingleScale(std::string fileName_, int scale_)
	: FA()
{
	fileName = fileName_;
	scale = scale_;
    checkFileExistence(fileName);
	N = setTsLength(fileName);
    allocateMemory();
}

HTsingleScale::~HTsingleScale(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<double>(F);
	delAlloc<double>(Ht);
}

void HTsingleScale::allocateMemory(){
	t = new double [N];
	y = new double [N];
	F = new double [getRangeLength(scale, N)];
	Ht = new double [getRangeLength(scale, N)];
}

int HTsingleScale::getTsLength(){
	return N;
}

std::string HTsingleScale::getFileName(){
    return fileName;
}
    
bool HTsingleScale::computeFlucVec(){
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(scale, N);
    ao.zeroVec(F, range);

    QProgressDialog progress(strHT+"\n"+"scale = "+QString::number(scale)+" -> "+
                             QString::fromStdString(fileName.substr(fileName.find_last_of("/")+1)),
                             "Stop", 0, range);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setFixedSize(xPG, yPG);

    //#pragma omp parallel for ---- no parallel for with graphic components or break statement
    for(int v = 0; v < range; v++){
        progress.setValue(v);
        if(progress.wasCanceled()){
            execStop = true;
            break;
        }
        int startLim = v;
        int endLim = v + scale - 1;
        double *tFit, *yFit, *diffVec;
        tFit = new double [scale];
        yFit = new double [scale];
        diffVec = new double [scale];
		ao.sliceVec(t, tFit, startLim, endLim);
		ao.sliceVec(y, yFit, startLim, endLim);
        double angCoeff, intercept;
		mo.linFit(scale, tFit, yFit, &angCoeff, &intercept);
		for(int j = 0; j < scale; j++)
			diffVec[j] = pow((yFit[j] - (intercept + angCoeff * tFit[j])), 2.0);
		F[v] = sqrt(mo.mean(diffVec, scale));
        delAlloc<double>(tFit);
        delAlloc<double>(yFit);
        delAlloc<double>(diffVec);
	}

    progress.setValue(range);

    return execStop;
}

void HTsingleScale::setMFDFAstep(int mfdfaStep)
{
    step = mfdfaStep;
}

void HTsingleScale::fitFlucVec(int start, int end)
{
    if(step == 0)
        step = 1;
    MFDFAsingleQ dfaQ0 = MFDFAsingleQ(fileName, start, end, 1, 0.0, step);
    dfaQ0.setFlucVectors();
    dfaQ0.computeFlucVec();
    dfaQ0.fitFlucVec(start, end);
    double Hq0 = dfaQ0.getH();
    double Hq0Intercept = dfaQ0.getHintercept();

    MathOps mo = MathOps();
    int range = getRangeLength(scale, N);
    double regfit, logscale;
    regfit = Hq0Intercept + Hq0 * log(scale);
    logscale = log(range) - log(scale);
    for(int i = 0; i < range; i++){
        Ht[i] = (regfit - log(F[i])) / static_cast<double>(logscale) + Hq0;
    }
}

std::string HTsingleScale::outFileStr(){
    return "/"+HTsSfnStart+"_"+std::to_string(scale)+"_"+fileName.substr(fileName.find_last_of("/")+1);
}

void HTsingleScale::saveFile(std::string pathTot){
    FileOps fo = FileOps();
	int range = getRangeLength(scale, N);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
	fprintf(f, "#scale=%d\n", scale);
    for(int i = 0; i < range; i++)
        fprintf(f, "%lf\n", Ht[i]);
    fclose(f);
}

void HTsingleScale::plot(BasePlot *plt)
{
    int len = getRangeLength(scale, N);
    QVector<double> pltVec(len), w(len);
    for(int i = 0; i < len; i++){
        w[i] = i + 1;
        pltVec[i] = Ht[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("w");
    plt->yAxis->setLabel("Ht");
    plt->graph(0)->setData(w, pltVec);
    plt->graph(0)->setLineStyle(QCPGraph::lsLine);
    QPen pen;
    pen.setColor(Qt::red);
    plt->graph(0)->setPen(pen);
    QString fn = QString::fromStdString(fileName).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    MathOps mo;
    plt->graph(0)->setName(fn+" - <Ht> = "+QString::number(mo.mean(Ht, len)));
    plt->graph(0)->rescaleAxes();
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}
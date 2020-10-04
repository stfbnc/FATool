#include "HT_single_scale.h"

HTsingleScale::HTsingleScale(std::string fileName, std::vector<double> ts, int tsLen, int scale) :
    FA(ts, tsLen)
{
    this->fileName = fileName;
    this->scale = scale;
    N = setTsLength(ts, tsLen);
}

HTsingleScale::~HTsingleScale(){}

QString HTsingleScale::getAlgorithmStr()
{
    return strHT;
}

int HTsingleScale::getTsLength()
{
	return N;
}

std::string HTsingleScale::getFileName()
{
    return fileName;
}
    
void HTsingleScale::executeAlgorithm()
{
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
    int range = getRangeLength(scale, N);

    for(int v = 0; v < range; v++)
    {
        emit progressSingle(v);

        int startLim = v;
        int endLim = v + scale - 1;
        std::vector<double> tFit = std::vector<double>();
        std::vector<double> yFit = std::vector<double>();
        std::vector<double> diffVec = std::vector<double>();
		ao.sliceVec(t, tFit, startLim, endLim);
		ao.sliceVec(y, yFit, startLim, endLim);
        double angCoeff, intercept;
		mo.linFit(scale, tFit, yFit, &angCoeff, &intercept);
		for(int j = 0; j < scale; j++)
            diffVec.push_back(pow((yFit.at(j) - (intercept + angCoeff * tFit.at(j))), 2.0));
        F.push_back(sqrt(mo.mean(diffVec, scale)));
	}
}

void HTsingleScale::setMFDFAstep(int mfdfaStep)
{
    step = mfdfaStep;
}

void HTsingleScale::executeFit(int start, int end)
{
    if(step == 0)
        step = 1;
    MFDFAsingleQ dfaQ0 = MFDFAsingleQ(fileName, ts, tsLen, start, end, 1, 0.0, step);
    dfaQ0.setVectors();
    dfaQ0.executeAlgorithm();
    dfaQ0.executeFit(start, end);
    double Hq0 = dfaQ0.getH();
    double Hq0Intercept = dfaQ0.getHintercept();

    Ht.clear();
    MathOps mo = MathOps();
    int range = getRangeLength(scale, N);
    double regfit, logscale;
    regfit = Hq0Intercept + Hq0 * log(scale);
    logscale = log(range) - log(scale);
    for(int i = 0; i < range; i++)
        Ht.push_back((regfit - log(F.at(i))) / static_cast<double>(logscale) + Hq0);
}

bool HTsingleScale::isFittable()
{
    return false;
}

int HTsingleScale::getLogType()
{
    return BasePlot::NO_AX;
}

std::string HTsingleScale::outFileStr()
{
    return "/" + HTsSfnStart + "_" + std::to_string(scale) + "_" +
            fileName.substr(fileName.find_last_of("/") + 1);
}

void HTsingleScale::saveFile(std::string pathTot)
{
    FileOps fo = FileOps();
	int range = getRangeLength(scale, N);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
	fprintf(f, "#scale=%d\n", scale);
    for(int i = 0; i < range; i++)
        fprintf(f, "%lf\n", Ht.at(i));
    fclose(f);
}

void HTsingleScale::plot(BasePlot *plt)
{
    int len = getRangeLength(scale, N);
    QVector<double> pltVec(len), w(len);
    for(int i = 0; i < len; i++)
    {
        w[i] = i + 1;
        pltVec[i] = Ht.at(i);
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
    plt->graph(0)->setName(fn + " - <Ht> = " + QString::number(mo.mean(Ht, len)));
    plt->graph(0)->rescaleAxes();
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

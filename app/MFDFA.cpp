#include "MFDFA.h"

MFDFA::MFDFA(std::string fileName, std::vector<double> ts, int tsLen, int minWin, int maxWin, int ord, double qIn, int Nq, int winStep, double stepq, int revSeg)
    : MFDFAsingleQ(fileName, ts, tsLen, minWin, maxWin, ord, qIn, winStep, revSeg)
{
    this->Nq = Nq;
    this->stepq = stepq;
}

MFDFA::~MFDFA(){}

void MFDFA::setQrange(double start, int len, double step)
{
    ArrayOps ao = ArrayOps();
    ao.doubleRange(qRange, len, start, step);
}

void MFDFA::executeAlgorithm()
{
	setQrange(q, Nq, stepq);
	int Lq = getRangeLength(minWin, maxWin, winStep);
    for(int i = 0; i < Lq; i++)
    {
        std::vector<double> vec = std::vector<double>();
        for(int j = 0; j < Nq; j++)
            vec.push_back(0.0);
        flucMtx.push_back(vec);
    }

    for(int i = 0; i < Nq; i++)
    {
        q = qRange.at(i);
        //execStop =
        MFDFAsingleQ::executeAlgorithm();
        //if(!execStop)
        //{
            for(int j = 0; j < Lq; j++)
                flucMtx.at(j).at(i) = getF().at(j);
            executeFit(minWin, maxWin);
            Hq.push_back(getH());
            Hinterceptq.push_back(getHintercept());
        /*}
        else
        {
            break;
        }*/
    }
}

void MFDFA::computeMassExponents()
{
    for(int i = 0; i < Nq; i++)
        tau.push_back(Hq.at(i) * qRange.at(i) - 1.0);
}

void MFDFA::computeSpectrum()
{
    computeMassExponents();
    for(int i = 0; i < Nq-1; i++)
        alpha.push_back((tau.at(i+1) - tau.at(i)) / (qRange.at(1) - qRange.at(0)));
    for(int i = 0; i < Nq-1; i++)
        spectrum.push_back(qRange.at(i) * alpha.at(i) - tau.at(i));
}

std::string MFDFA::outFileStr(){
    return "/" + MFDFAfnStart + "_" + std::to_string(minWin) +
            "_" + std::to_string(maxWin) + "_q" +
            std::to_string(static_cast<int>(qRange.at(0))) + "_" +
            std::to_string(static_cast<int>(qRange.at(Nq-1))) +
            "_" + fileName.substr(fileName.find_last_of("/") + 1);
}

void MFDFA::saveFile(std::string pathTot)
{
	FileOps fo = FileOps();
	int Lq = getRangeLength(minWin, maxWin, winStep);
	FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
    fprintf(f, "# q ");
    for(int i = 0; i < Nq; i++)
        i == Nq - 1 ? fprintf(f, "%lf\n", qRange.at(i)) : fprintf(f, "%lf ", qRange.at(i));

    fprintf(f, "# scale F_q\n");
    for(int i = 0; i < Lq; i++)
    {
        fprintf(f, "%d ", s.at(i));
        for(int j = 0; j < Nq; j++)
            j == Nq - 1 ? fprintf(f, "%lf\n", flucMtx.at(i).at(j)) : fprintf(f, "%lf ", flucMtx.at(i).at(j));
	}
    fclose(f);

    qsaveFile(pathTot);
}

std::string MFDFA::qoutFileStr()
{
    return "/" + MFDFAfnStart + "_q" + std::to_string(static_cast<int>(qRange.at(0))) +
            "_" + std::to_string(static_cast<int>(qRange.at(Nq - 1))) +
            "_" + fileName.substr(fileName.find_last_of("/") + 1);
}

void MFDFA::qsaveFile(std::string pathTot)
{
	FileOps fo = FileOps();
	FILE *f;
    f = fo.openFile(pathTot+qoutFileStr(), "w");
    fprintf(f, "# q hq hq_intercept\n");
    for(int i = 0; i < Nq; i++)
        fprintf(f, "%lf %lf %lf\n", qRange.at(i), Hq.at(i), Hinterceptq.at(i));
    fclose(f);
}

std::string MFDFA::tauOutFileStr()
{
    return "/tau_q" + std::to_string(static_cast<int>(qRange.at(0))) +
            "_" + std::to_string(static_cast<int>(qRange.at(Nq - 1))) +
            "_" + fileName.substr(fileName.find_last_of("/") + 1);
}

void MFDFA::tauSaveFile(std::string pathTot)
{
    FileOps fo = FileOps();
    FILE *f;
    f = fo.openFile(pathTot+tauOutFileStr(), "w");
    fprintf(f, "# q tau\n");
    for(int i = 0; i < Nq; i++)
        fprintf(f, "%lf %lf\n", qRange.at(i), tau.at(i));
    fclose(f);
}

std::string MFDFA::spectrumOutFileStr()
{
    return "/spectrum_q" + std::to_string(static_cast<int>(qRange.at(0))) +
            "_" + std::to_string(static_cast<int>(qRange.at(Nq - 1))) +
            "_" + fileName.substr(fileName.find_last_of("/") + 1);
}

void MFDFA::spectrumSaveFile(std::string pathTot)
{
    FileOps fo = FileOps();
    FILE *f;
    f = fo.openFile(pathTot+spectrumOutFileStr(), "w");
    fprintf(f, "# alpha spectrum\n");
    for(int i = 0; i < Nq - 1; i++)
        fprintf(f, "%lf %lf\n", alpha.at(i), spectrum.at(i));
    fclose(f);
}

void MFDFA::plot(BasePlot *plt)
{
    QVector<double> yh(Nq), xq(Nq);
    for(int i = 0; i < Nq; i++)
    {
        xq[i] = qRange.at(i);
        yh[i] = Hq.at(i);
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
    for(int i = 0; i < Nq; i++)
    {
        xq[i] = qRange.at(i);
        yt[i] = tau.at(i);
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
    QVector<double> a(Nq - 1), f(Nq - 1);
    for(int i = 0; i < Nq - 1; i++)
    {
        a[i] = alpha.at(i);
        f[i] = spectrum.at(i);
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

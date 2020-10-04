#include "rhoDCCA.h"

rhoDCCA::rhoDCCA(std::string fileName, std::vector<double> ts, int tsLen, std::string fileName2, std::vector<double> ts2, int tsLen2, int minWin, int maxWin, int ord, int winStep, bool thresh) :
    FA(ts, tsLen)
{
    this->fileName = fileName;
    this->fileName2 = fileName2;
    this->tsLen2 = tsLen2;
    this->ts2.reserve(tsLen2);
    for(int i = 0; i < tsLen2; i++)
        this->ts2.push_back(ts2.at(i));
    this->minWin = minWin;
    this->maxWin = maxWin;
    this->ord = ord;
    this->winStep = winStep;
    this->thresh = thresh;
    L = 0;
}

rhoDCCA::~rhoDCCA() {}

QString rhoDCCA::getAlgorithmStr()
{
    return strRHODCCA;
}

void rhoDCCA::executeAlgorithm()
{
    DCCA dccaXX = DCCA(fileName, ts, tsLen, fileName, ts, tsLen, minWin, maxWin, ord, defaultDCCA.toStdString(), winStep, false);
    dccaXX.setVectors();
    connect(&dccaXX, &DCCA::progressSingle, [&](int val){ emit progress(val); });
    dccaXX.executeAlgorithm();
    std::vector<double> Fxx = dccaXX.getF();

    DCCA dccaYY = DCCA(fileName2, ts2, tsLen2, fileName2, ts2, tsLen2, minWin, maxWin, ord, defaultDCCA.toStdString(), winStep, false);
    dccaYY.setVectors();
    connect(&dccaYY, &DCCA::progressSingle, [&](int val){ emit progress(val + getRangeLength(minWin, maxWin, winStep)); });
    dccaYY.executeAlgorithm();
    std::vector<double> Fyy = dccaYY.getF();

    DCCA dccaXY = DCCA(fileName, ts, tsLen, fileName2, ts2, tsLen2, minWin, maxWin, ord, corrDCCA.toStdString(), winStep, false);
    dccaXY.setVectors();
    connect(&dccaXY, &DCCA::progressSingle, [&](int val){ emit progress(val + 2 * getRangeLength(minWin, maxWin, winStep)); });
    dccaXY.executeAlgorithm();
    std::vector<double> Fxy = dccaXY.getF();

    L = dccaXY.getRangeLength(minWin, maxWin, winStep);
    N = dccaXY.getTsLength();
    for(int i = 0; i < L; i++)
        rho.push_back(Fxy.at(i) / static_cast<double>(Fxx.at(i) * Fyy.at(i)));

    emit progress(1 + 3 * getRangeLength(minWin, maxWin, winStep));

    if(thresh)
        computeThresholds();

    emit progress(getAlgorithmTotalSteps());
    emit executionEnded(this);
}

void rhoDCCA::computeThresholds()
{
    MathOps mo;
    ArrayOps ao;
    std::vector<double> rhoMtx = std::vector<double>();
    ao.zeroVec(rhoMtx, getRhoLength() * nSim);

    for(int i = 0; i < nSim; i++)
    {
        std::random_device rd1{};
        std::mt19937 gen1{rd1()};
        std::random_device rd2{};
        std::mt19937 gen2{rd2()};

        std::vector<double> grand1 = std::vector<double>();
        std::vector<double> grand2 = std::vector<double>();
        mo.gaussRand(grand1, N, gen1);
        mo.gaussRand(grand2, N, gen2);

        DCCA dcca11 = DCCA("", grand1, N, "", grand1, N, minWin, maxWin, ord, defaultDCCA.toStdString(), winStep, false);
        dcca11.setVectors();
        dcca11.executeAlgorithm();
        std::vector<double> F11 = dcca11.getF();
        emit progress(1 + 3 * getRangeLength(minWin, maxWin, winStep) + i * 3);

        DCCA dcca22 = DCCA("", grand2, N, "", grand2, N, minWin, maxWin, ord, defaultDCCA.toStdString(), winStep, false);
        dcca22.setVectors();
        dcca22.executeAlgorithm();
        std::vector<double> F22 = dcca22.getF();
        emit progress(1 + 3 * getRangeLength(minWin, maxWin, winStep) + i * 3 + 1);

        DCCA dcca12 = DCCA("", grand1, N, "", grand2, N, minWin, maxWin, ord, corrDCCA.toStdString(), winStep, false);
        dcca12.setVectors();
        dcca12.executeAlgorithm();
        std::vector<double> F12 = dcca12.getF();
        emit progress(1 + 3 * getRangeLength(minWin, maxWin, winStep) + i * 3 + 2);

        for(int j = 0; j < getRhoLength(); j++)
            rhoMtx.at(i*getRhoLength()+j) = F12.at(j) / static_cast<double>(F11.at(j) * F22.at(j));
    }

    confLevels(rhoMtx);
}

void rhoDCCA::confLevels(std::vector<double> rhos)
{
    MathOps mo;
    ArrayOps ao;
    for(int i = 0; i < getRhoLength(); i++)
    {
        std::vector<double> singleCol = std::vector<double>();
        ao.extractColumn(rhos, getRhoLength(), nSim, i, singleCol);
        std::sort(singleCol.begin(), singleCol.end());
        confUp.push_back(singleCol[mo.quantile(nSim, confLev)]);
        confDown.push_back(singleCol[mo.quantile(nSim, 1-confLev)]);
    }
}

int rhoDCCA::getRhoLength()
{
    return L;
}

bool rhoDCCA::threshCompute()
{
    return thresh;
}

std::string rhoDCCA::getFileName()
{
    return fileName;
}

std::string rhoDCCA::getFileName2()
{
    return fileName2;
}

int rhoDCCA::getMinWin()
{
    return minWin;
}

int rhoDCCA::getMaxWin()
{
    return maxWin;
}

bool rhoDCCA::isFittable()
{
    return false;
}

int rhoDCCA::getLogType()
{
    return BasePlot::NO_AX;
}

std::string rhoDCCA::outFileStr()
{
    size_t a = fileName.find_last_of("/");
    size_t b = fileName.find_last_of(".");
    return "/" + RHODCCAfnStart + "_" + std::to_string(minWin) + "_" +
            std::to_string(maxWin) + "_" + fileName.substr(a + 1, b - a - 1) +
            "_" + fileName2.substr(fileName2.find_last_of("/") + 1);
}

void rhoDCCA::saveFile(std::string pathTot)
{
    FileOps fo = FileOps();
    FILE *f;
    f = fo.openFile(pathTot+outFileStr(), "w");
    if(thresh)
        fprintf(f, "# scale rho lower_conf upper_conf\n");
    else
        fprintf(f, "# scale rho\n");
    for(int i = 0; i < getRhoLength(); i++)
    {
        if(thresh)
            fprintf(f, "%d %lf %lf %lf\n", (i * winStep) + minWin, rho.at(i), confDown.at(i), confUp.at(i));
        else
            fprintf(f, "%d %lf\n", (i * winStep) + minWin, rho.at(i));
    }
    fclose(f);
}

void rhoDCCA::plot(BasePlot *plt)
{
    QVector<double> n(L), corrs(L);
    for(int i = 0; i < L; i++)
    {
        n[i] = (i * winStep) + minWin;
        corrs[i] = rho.at(i);
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
    if(thresh)
    {
        QVector<double> cd(L), cu(L);
        for(int i = 0; i < L; i++)
        {
            cd[i] = confDown.at(i);
            cu[i] = confUp.at(i);
        }
        pen.setColor(Qt::black);
        pen.setWidth(2);
        plt->addGraph();
        plt->graph(1)->setData(n, cd);
        plt->graph(1)->setPen(pen);
        plt->graph(1)->setName("Lower " + QString::number(confLev * 100.0) + "% CI");
        plt->graph(1)->rescaleAxes(true);
        plt->addGraph();
        plt->graph(2)->setData(n, cu);
        plt->graph(2)->setPen(pen);
        plt->graph(2)->setName("Upper " + QString::number(confLev * 100.0) + "% CI");
        plt->graph(2)->rescaleAxes(true);
    }
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

int rhoDCCA::getAlgorithmTotalSteps()
{
    int totLen = getRangeLength(minWin, maxWin, winStep) * 3 + 1;
    if(thresh)
        totLen += nSim * 3;

    return totLen;
}

std::string rhoDCCA::getCurrentIdentifier()
{
    return fileName.substr(fileName.find_last_of("/") + 1) +
           "\nVS\n" +
           fileName2.substr(fileName2.find_last_of("/") + 1);
}

#include "HT.h"

HT::HT(std::string fileName, std::vector<double> ts, int tsLen, int scale, int Nscales, int stepScale, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep) :
    HTsingleScale(fileName, ts, tsLen, scale)
{
    setMFDFAstep(mfdfaStep);
    this->mfdfaMinWin = mfdfaMinWin;
    this->mfdfaMaxWin = mfdfaMaxWin;
    this->Nscales = Nscales;
    minScale = scale;
    this->stepScale = stepScale;
    ArrayOps ao = ArrayOps();
    ao.intRange(scales, Nscales, minScale, stepScale);
}

HT::HT(std::string fileName, std::vector<double> ts, int tsLen, std::string strScales, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep) :
    HTsingleScale(fileName, ts, tsLen, stoi(strScales.substr(0, strScales.find_first_of(strSep))))
{
    setMFDFAstep(mfdfaStep);
    this->mfdfaMinWin = mfdfaMinWin;
    this->mfdfaMaxWin = mfdfaMaxWin;
	Nscales = getNumScales(strScales);
    minScale = getMinScale(strScales);
	getScales(strScales);
}

HT::~HT(){}

int HT::getNumScales(std::string str)
{
	int count = 0;
    size_t pos = 0;
    while((pos = str.find(strSep, pos)) != std::string::npos)
    {
	    count++;
	    pos++;
	}
	count++;

	return count;
}

int HT::getMinScale(std::string str)
{
    std::string strCpy = str;
    int min = 0;
    int i = 0;
    size_t pos = 0;
    std::string token;
    if(strCpy.find(strSep) != std::string::npos)
    {
        while((pos = strCpy.find(strSep)) != std::string::npos)
        {
            token = strCpy.substr(0, pos);
            int s = stoi(token);
            if(i == 0)
            {
                min = s;
                i++;
            }
            else
            {
                if(s < min)
                    min = s;
            }
            strCpy.erase(0, pos+strSep.length());
        }
    }
    else
    {
        min = stoi(strCpy);
    }

    return min;
}

void HT::getScales(std::string str)
{
    size_t pos = 0;
	std::string token;
    while((pos = str.find(strSep)) != std::string::npos)
    {
	    token = str.substr(0, pos);
        int s = stoi(token);
        if(s > N)
            scales.push_back(N);
        else
            scales.push_back(s);
        str.erase(0, pos + strSep.length());
	}
    scales.push_back(stoi(str));
}

bool HT::executeAlgorithm(){
    bool execStop = false;
	int L = getRangeLength(minScale, N);
	ArrayOps ao = ArrayOps();
    for(int i = 0; i < L; i++)
    {
        std::vector<double> vec = std::vector<double>();
        for(int j = 0; j < Nscales; j++)
            vec.push_back(0.0);
        HTmtx.push_back(vec);
    }

    for(int i = 0; i < Nscales; i++)
    {
        scale = scales.at(i);
        int Lscale = getRangeLength(scale, N);
        execStop = HTsingleScale::executeAlgorithm();
        if(!execStop)
        {
            executeFit(mfdfaMinWin, mfdfaMaxWin);
            for(int j = 0; j < Lscale; j++)
                HTmtx.at(j).at(i) = Ht.at(j);
            for(int j = Lscale; j < L; j++)
                HTmtx.at(j).at(i) = std::numeric_limits<double>::quiet_NaN();
        }
        else
        {
            break;
        }
	}

    return execStop;
}

std::string HT::outFileStr()
{
    return "/" + HTfnStart + "_" +
            fileName.substr(fileName.find_last_of("/") + 1);
}

void HT::saveFile(std::string pathTot)
{
	FileOps fo = FileOps();
	int L = getRangeLength(minScale, N);
	FILE *f;
    f = fo.openFile(pathTot + outFileStr(), "w");
	fprintf(f, "#scale ");
    for(int i = 0; i < Nscales; i++)
        i == Nscales - 1 ? fprintf(f, "%d\n", scales.at(i)) : fprintf(f, "%d ", scales.at(i));

    for(int i = 0; i < L; i++)
        for(int j = 0; j < Nscales; j++)
            j == Nscales - 1 ? fprintf(f, "%lf\n", HTmtx.at(i).at(j)) : fprintf(f, "%lf ", HTmtx.at(i).at(j));

	fclose(f);
}

void HT::plot(BasePlot *plt)
{
    QString fn = QString::fromStdString(fileName).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    int L = getRangeLength(minScale, N);
    QVector<double> w(L);
    for(int i = 0; i < L; i++)
        w[i] = i + 1;
    QVector<double> pltVec(L); 
    for(int j = 0; j < Nscales; j++)
    {
        std::vector<double> vec = std::vector<double>();
        for(int i = 0; i < L; i++)
        {
            pltVec[i] = HTmtx.at(i).at(j);
            vec.push_back(HTmtx.at(i).at(j));
        }
        plt->addGraph();
        plt->graph(j)->setData(w, pltVec);
        plt->graph(j)->setLineStyle(QCPGraph::lsLine);
        QPen pen;
        pen.setColor(colours.at(j % colours.size()));
        plt->graph(j)->setPen(pen);
        MathOps mo;
        plt->graph(j)->setName(fn + "_w" + QString::number(scales.at(j)) + " - <Ht> = " + QString::number(mo.nanMean(vec, L)));
        j==0 ? plt->graph(j)->rescaleAxes() : plt->graph(j)->rescaleAxes(true);
    }
    plt->xAxis->setLabel("w");
    plt->yAxis->setLabel("Ht");
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

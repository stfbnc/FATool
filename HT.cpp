#include "HT.h"

HT::HT(std::string fileName_, double *ts_, int tsLen_, int scale_, int Nscales_, int stepScale_, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep)
    : HTsingleScale(fileName_, ts_, tsLen_, scale_)
{
    setMFDFAstep(mfdfaStep);
    mMFDFA = mfdfaMinWin;
    MMFDFA = mfdfaMaxWin;
	Nscales = Nscales_;
	minScale = scale_;
	stepScale = stepScale_;
    scales = nullptr;
    HTmtx = nullptr;
    allocateScalesMemory();
    ArrayOps ao = ArrayOps();
    ao.intRange(scales, Nscales, minScale, stepScale);
}

HT::HT(std::string fileName_, double *ts_, int tsLen_, std::string strScales, int mfdfaMinWin, int mfdfaMaxWin, int mfdfaStep)
        : HTsingleScale(fileName_, ts_, tsLen_, stoi(strScales.substr(0, strScales.find_first_of(strSep))))
{
    setMFDFAstep(mfdfaStep);
    mMFDFA = mfdfaMinWin;
    MMFDFA = mfdfaMaxWin;
	Nscales = getNumScales(strScales);
    minScale = getMinScale(strScales);
    scales = nullptr;
    HTmtx = nullptr;
	allocateScalesMemory();
	getScales(strScales);
}

HT::~HT(){
	delAlloc<int>(scales);
	del2Alloc<double>(HTmtx, getRangeLength(minScale, N));
}

void HT::allocateScalesMemory(){
	scales = new int [Nscales];
	HTmtx = new double* [getRangeLength(minScale, N)];
    for(int i = 0; i < getRangeLength(minScale, N); i++)
		HTmtx[i] = new double [Nscales];
}

int HT::getNumScales(std::string str){
	int count = 0;
    size_t pos = 0;
	while((pos = str.find(strSep, pos)) != std::string::npos){
	    count++;
	    pos++;
	}
	count++;
	return count;
}

int HT::getMinScale(std::string str){
    std::string strCpy = str;
    int min = 0;
    int i = 0;
    size_t pos = 0;
    std::string token;
    if(strCpy.find(strSep) != std::string::npos){
        while((pos = strCpy.find(strSep)) != std::string::npos){
            token = strCpy.substr(0, pos);
            int s = stoi(token);
            if(i == 0){
                min = s;
                i++;
            }else{
                if(s < min)
                    min = s;
            }
            strCpy.erase(0, pos+strSep.length());
        }
    }else
        min = stoi(strCpy);
    return min;
}

void HT::getScales(std::string str){
	int i = 0;
    size_t pos = 0;
	std::string token;
	while((pos = str.find(strSep)) != std::string::npos){
	    token = str.substr(0, pos);
        int s = stoi(token);
        if(s > N)
            scales[i] = N;
        else
            scales[i] = s;
	    str.erase(0, pos+strSep.length());
	    i++;
	}
	scales[i] = stoi(str);
}

bool HT::computeFlucVec(){
    bool execStop = false;
	int L = getRangeLength(minScale, N);
	ArrayOps ao = ArrayOps();
	for(int i = 0; i < Nscales; i++){
		scale = scales[i];
		int Lscale = getRangeLength(scale, N);
        ao.zeroVec(Ht, L);
        execStop = HTsingleScale::computeFlucVec();
        if(!execStop){
            fitFlucVec(mMFDFA, MMFDFA);
            for(int j = 0; j < Lscale; j++)
                HTmtx[j][i] = Ht[j];
            for(int j = Lscale; j < L; j++)
                HTmtx[j][i] = std::numeric_limits<double>::quiet_NaN();
        }else
            break;
	}
    return execStop;
}

std::string HT::outFileStr(){
	return "/"+HTfnStart+"_"+fileName.substr(fileName.find_last_of("/")+1);
}

void HT::saveFile(std::string pathTot){
	FileOps fo = FileOps();
	int L = getRangeLength(minScale, N);
	FILE *f;
	f = fo.openFile(pathTot+outFileStr(), "w");
	fprintf(f, "#scale ");
	for(int i = 0; i < Nscales; i++){
		i == Nscales-1 ? fprintf(f, "%d\n", scales[i]) : fprintf(f, "%d ", scales[i]);
	}
	for(int i = 0; i < L; i++){
		for(int j = 0; j < Nscales; j++){
			j == Nscales-1 ? fprintf(f, "%lf\n", HTmtx[i][j]) : fprintf(f, "%lf ", HTmtx[i][j]);
		}
	}
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
    double *vec;
    vec = new double [L];
    for(int j = 0; j < Nscales; j++){
        for(int i = 0; i < L; i++){
            pltVec[i] = HTmtx[i][j];
            vec[i] = HTmtx[i][j];
        }
        plt->addGraph();
        plt->graph(j)->setData(w, pltVec);
        plt->graph(j)->setLineStyle(QCPGraph::lsLine);
        QPen pen;
        pen.setColor(colours[j%colours.size()]);
        plt->graph(j)->setPen(pen);
        MathOps mo;
        plt->graph(j)->setName(fn+"_w"+QString::number(scales[j])+" - <Ht> = "+QString::number(mo.nanMean(vec, L)));
        j==0 ? plt->graph(j)->rescaleAxes() : plt->graph(j)->rescaleAxes(true);
    }
    plt->xAxis->setLabel("w");
    plt->yAxis->setLabel("Ht");
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    delAlloc(vec);
}

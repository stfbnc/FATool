#include "HT.h"

HT::HT(string file_name_, int scale_, int Nscales_, int stepScale_)
	: HTsingleScale(file_name_, scale_)
{
	Nscales = Nscales_;
	minScale = scale_;
	stepScale = stepScale_;
    scales = nullptr;
    HTmtx = nullptr;
    allocateScalesMemory();
    ArrayOps ao = ArrayOps();
    ao.int_range(scales, Nscales, minScale, stepScale);
}

HT::HT(string fileName, string strScales)
		: HTsingleScale(fileName, stoi(strScales.substr(0, strScales.find_first_of(STR_SEP))))
{
	Nscales = getNumScales(strScales);
    scales = nullptr;
    HTmtx = nullptr;
	allocateScalesMemory();
	getScales(strScales);
	MathOps mo = MathOps();
	minScale = mo.vec_min(scales, Nscales);
}

HT::~HT(){
	delAlloc<int>(scales);
	del2Alloc<double>(HTmtx, getRangeLength(minScale, N));
}

void HT::allocateScalesMemory(){
	scales = new int [Nscales];
	HTmtx = new double* [getRangeLength(minScale, N)];
	for(int i = 0; i < getRangeLength(minScale, N); i++){
		HTmtx[i] = new double [Nscales];
	}
}

int HT::getNumScales(string str){
	int count = 0;
    size_t pos = 0;
	while((pos = str.find(STR_SEP, pos)) != string::npos){
	    count++;
	    pos++;
	}
	count++;
	return count;
}

void HT::getScales(string str){
	int i = 0;
    size_t pos = 0;
	string token;
	while((pos = str.find(STR_SEP)) != string::npos){
	    token = str.substr(0, pos);
        int s = stoi(token);
        if(s > N)
            scales[i] = N;
        else
            scales[i] = s;
	    str.erase(0, pos+STR_SEP.length());
	    i++;
	}
	scales[i] = stoi(str);
}

bool HT::winFlucComp(){
    bool execStop = false;
	int L = getRangeLength(minScale, N);
	ArrayOps ao = ArrayOps();
	for(int i = 0; i < Nscales; i++){
		scale = scales[i];
		int Lscale = getRangeLength(scale, N);
		ao.zero_vec(Ht, L);
        execStop = HTsingleScale::winFlucComp();
        if(!execStop){
            Ht_fit();
            for(int j = 0; j < Lscale; j++)
                HTmtx[j][i] = Ht[j];
            for(int j = Lscale; j < L; j++)
                HTmtx[j][i] = numeric_limits<double>::quiet_NaN();
        }else
            break;
	}
    return execStop;
}

string HT::outFileStr(){
	return "/"+HT_FN_START+"_"+file_name.substr(file_name.find_last_of("/")+1);
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void HT::saveFile(string path_tot){
	FileOps fo = FileOps();
	int L = getRangeLength(minScale, N);
	FILE *f;
	f = fo.open_file(path_tot+outFileStr(), "w");
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

void HT::plot(QCustomPlot *plt)
{
    QString fn = QString::fromStdString(file_name).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    int L = getRangeLength(minScale, N);
//    QVector<double> w(L);
//    for(int i = 0; i < L; i++)
//        w[i] = i;
//    //QVector<double> plt_vec(L);
//    QVector<QCPGraphData> plt_vec(L);
//    double *vec;
//    vec = new double [L];
//    QVector<QCPAxisRect *> axRect(Nscales);
//    for(int j = 0; j < Nscales; j++){
//        axRect[j] = new QCPAxisRect(plt);
//        plt->plotLayout()->addElement(j, 0, axRect[j]);
//        for(int i = 0; i < L; i++){
//            plt_vec[i].key = i + 1;
//            plt_vec[i].value = HTmtx[i][j];
//            vec[i] = HTmtx[i][j];
//        }
//        QCPGraph *gr = plt->addGraph(axRect[j]->axis(QCPAxis::atBottom, j), axRect[j]->axis(QCPAxis::atLeft));
//        gr->data()->set(plt_vec);
//        gr->rescaleKeyAxis();
//        gr->setLineStyle(QCPGraph::lsLine);
//        QPen pen;
//        pen.setColor(colours[j%colours.size()]);
//        gr->setPen(pen);
//        MathOps mo;
//        gr->setName(fn+"_w"+QString::number(scales[j])+" - <Ht> = "+QString::number(mo.nan_mean(vec, L)));
//        plt->graph(j)->setData(w, plt_vec);
//        plt->graph(j)->setLineStyle(QCPGraph::lsLine);
//        QPen pen;
//        pen.setColor(colours[j%colours.size()]);
//        plt->graph(j)->setPen(pen);
//        MathOps mo;
//        plt->graph(j)->setName(fn+"_w"+QString::number(scales[j])+" - <Ht> = "+QString::number(mo.nan_mean(vec, L)));
//        j==0 ? plt->graph(j)->rescaleAxes() : plt->graph(j)->rescaleAxes(true);
//    }
//    plt->xAxis->setLabel("w");
//    plt->yAxis->setLabel("Ht");
//    plt->legend->setVisible(true);
//    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
//    delAlloc(vec);
}

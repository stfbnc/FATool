#include "MFDFA.h"

MFDFA::MFDFA(string file_name_, int min_win_, int max_win_, int ord_, double qIn_, int Nq_, int win_step_, double stepq_, int rev_seg_)
	: MFDFAsingleQ(file_name_, min_win_, max_win_, ord_, qIn_, win_step_, rev_seg_)
{
	Nq = Nq_;
	stepq = stepq_;
    qRange = nullptr;
    flucMtx = nullptr;
    Hq = nullptr;
    H_interceptq = nullptr;
    allocateQmemory();
}

MFDFA::~MFDFA(){
	delAlloc<double>(qRange);
	delAlloc<double>(Hq);
	delAlloc<double>(H_interceptq);
	del2Alloc<double>(flucMtx, getRangeLength(min_win, max_win, win_step));
}

void MFDFA::allocateQmemory(){
    qRange = new double [Nq];
    Hq = new double [Nq];
    H_interceptq = new double [Nq];
    flucMtx = new double* [getRangeLength(min_win, max_win, win_step)];
    for(int i = 0; i < getRangeLength(min_win, max_win, win_step); i++){
    	flucMtx[i] = new double [Nq];
    }
}

void MFDFA::setQrange(double start, int len, double step){
    ArrayOps ao = ArrayOps();
    ao.double_range(qRange, len, start, step);
}

bool MFDFA::winFlucComp(){
    bool execStop = false;
	setQrange(q, Nq, stepq);
	int Lq = getRangeLength(min_win, max_win, win_step);
	for(int i = 0; i < Nq; i++){
		q = qRange[i];
        execStop = MFDFAsingleQ::winFlucComp();
        if(!execStop){
            for(int j = 0; j < Lq; j++){
                flucMtx[j][i] = F[j];
            }
            H_loglogFit(min_win, max_win);
            Hq[i] = getH();
            H_interceptq[i] = getH_intercept();
        }else
            break;
	}
    return execStop;
}

string MFDFA::outFileStr(){
    return "/"+MFDFA_FN_START+"_"+to_string(min_win)+"_"+to_string(max_win)+"_q"+to_string(static_cast<int>(qRange[0]))+"_"+
            to_string(static_cast<int>(qRange[Nq-1]))+"_"+file_name.substr(file_name.find_last_of("/")+1);
}

void MFDFA::saveFile(string path_tot){
	FileOps fo = FileOps();
	int Lq = getRangeLength(min_win, max_win, win_step);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
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

string MFDFA::qoutFileStr(){
    return "/"+MFDFA_FN_START+"_q"+to_string(static_cast<int>(qRange[0]))+"_"+to_string(static_cast<int>(qRange[Nq-1]))+
			"_"+file_name.substr(file_name.find_last_of("/")+1);
}

void MFDFA::qsaveFile(string path_tot){
	FileOps fo = FileOps();
	FILE *f;
    f = fo.open_file(path_tot+qoutFileStr(), "w");
    for(int i = 0; i < Nq; i++){
		fprintf(f, "%lf %lf %lf\n", qRange[i], Hq[i], H_interceptq[i]);
	}
    fclose(f);
}

void MFDFA::plot(QCustomPlot *plt){
    QVector<double> y_h(Nq), x_q(Nq);
    for(int i = 0; i < Nq; i++){
        x_q[i] = qRange[i];
        y_h[i] = Hq[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("q");
    plt->yAxis->setLabel("h(q)");
    plt->graph(0)->setData(x_q, y_h);
    plt->graph(0)->setLineStyle(QCPGraph::lsLine);
    plt->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::red, 10));
    QString fn = QString::fromStdString(file_name).split("/").last();
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

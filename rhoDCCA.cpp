#include "rhoDCCA.h"

rhoDCCA::rhoDCCA(string file_name_, string file_name2_, int min_win_, int max_win_, int ord_, int win_step_){
	file_name = file_name_;
	file_name2 = file_name2_;
	min_win = min_win_;
	max_win = max_win_;
	ord = ord_;
	win_step = win_step_;
	L = 0;
    rho = nullptr;
}

rhoDCCA::~rhoDCCA() {
	delete [] rho;
}

void rhoDCCA::computeRho(){
    DCCA dccaXX = DCCA(file_name, file_name, min_win, max_win, ord, DEFAULT_DCCA, win_step);
    dccaXX.setFlucVectors();
    dccaXX.winFlucComp();
    double *Fxx = dccaXX.getF();
    DCCA dccaYY = DCCA(file_name2, file_name2, min_win, max_win, ord, DEFAULT_DCCA, win_step);
    dccaYY.setFlucVectors();
    dccaYY.winFlucComp();
    double *Fyy = dccaYY.getF();
    DCCA dccaXY = DCCA(file_name, file_name2, min_win, max_win, ord, CORR_DCCA, win_step);
    dccaXY.setFlucVectors();
    dccaXY.winFlucComp();
    double *Fxy = dccaXY.getF();
    L = dccaXY.getRangeLength(min_win, max_win, win_step);
    rho = new double [L];
    for(int i = 0; i < L; i++)
        rho[i] = Fxy[i] / static_cast<double>(Fxx[i] * Fyy[i]);
}

int rhoDCCA::getRhoLength(){
    return L;
}

string rhoDCCA::outFileStr(){
    size_t a = file_name.find_last_of("/");
    size_t b = file_name.find_last_of(".");
    return "/"+RHODCCA_FN_START+"_"+to_string(min_win)+"_"+to_string(max_win)+"_"
            +file_name.substr(a+1, b-a-1)+"_"
			+file_name2.substr(file_name2.find_last_of("/")+1);
}

void rhoDCCA::saveFile(string path_tot){
    FileOps fo = FileOps();
    FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
    for(int i = 0; i < getRhoLength(); i++)
        fprintf(f, "%d %lf\n", (i*win_step)+min_win, rho[i]);
    fclose(f);
}

void rhoDCCA::plot(QCustomPlot *plt){
    QVector<double> n(L), corrs(L);
    for(int i = 0; i < L; i++){
        n[i] = (i * win_step) + min_win;
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
    QString fn = QString::fromStdString(file_name).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    QString fn2 = QString::fromStdString(file_name2).split("/").last();
    fn2.truncate(fn2.lastIndexOf("."));
    plt->graph(0)->setName(fn+"_"+fn2+"_"+QString::number(min_win)+"_"+QString::number(max_win));
    plt->graph(0)->rescaleAxes();
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

#include "HTsingleScale.h"
#include "MFDFAsingleQ.h"

HTsingleScale::HTsingleScale(string file_name_, int scale_)
	: FA()
{
	file_name = file_name_;
	scale = scale_;
    checkFileExistence(file_name);
	N = setTsLength(file_name);
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
    
bool HTsingleScale::winFlucComp(){
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(scale, N);
//    double *t_fit, *y_fit, *diff_vec;
//    t_fit = new double [scale];
//    y_fit = new double [scale];
//    diff_vec = new double [scale];
    //computation
//    int start_lim, end_lim;
//    double ang_coeff, intercept;
//	ao.zero_vec(t_fit, scale);
//	ao.zero_vec(y_fit, scale);
//	ao.zero_vec(diff_vec, scale);
	ao.zero_vec(F, range);
    QProgressDialog progress(strHT+"\n"+"scale = "+QString::number(scale)+" -> "+
                             QString::fromStdString(file_name.substr(file_name.find_last_of("/")+1)),
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
        int start_lim = v;
        int end_lim = v + scale - 1;
        double *t_fit, *y_fit, *diff_vec;
        t_fit = new double [scale];
        y_fit = new double [scale];
        diff_vec = new double [scale];
		ao.slice_vec(t, t_fit, start_lim, end_lim);
		ao.slice_vec(y, y_fit, start_lim, end_lim);
        double ang_coeff, intercept;
		mo.lin_fit(scale, t_fit, y_fit, &ang_coeff, &intercept);
		for(int j = 0; j < scale; j++)
			diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
		F[v] = sqrt(mo.mean(diff_vec, scale));
        delAlloc<double>(t_fit);
        delAlloc<double>(y_fit);
        delAlloc<double>(diff_vec);
	}
    progress.setValue(range);
//    delAlloc<double>(t_fit);
//    delAlloc<double>(y_fit);
//    delAlloc<double>(diff_vec);
    return execStop;
}

void HTsingleScale::H_loglogFit(int a, int b){
    a = 0;
    b = 0;
}

void HTsingleScale::Ht_fit()
{
    int start = 4;
    int end = N / 5;
    int step = N / 20;
    if(step == 0)
        step = 1;
    MFDFAsingleQ dfaQ0 = MFDFAsingleQ(file_name, start, end, 1, 0.0, step);
    dfaQ0.setFlucVectors();
    dfaQ0.winFlucComp();
    dfaQ0.H_loglogFit(start, end);
    double Hq0 = dfaQ0.getH();
    double Hq0_intercept = dfaQ0.getH_intercept();

    MathOps mo = MathOps();
    int range = getRangeLength(scale, N);
    double Regfit, logscale;
    Regfit = Hq0_intercept + Hq0 * log(scale);
    logscale = log(range) - log(scale);
    for(int i = 0; i < range; i++){
        Ht[i] = (Regfit - log(F[i])) / static_cast<double>(logscale) + Hq0;
    }
}

string HTsingleScale::outFileStr(){
	return "/"+HTsS_FN_START+"_"+to_string(scale)+"_"+file_name.substr(file_name.find_last_of("/")+1);
}

void HTsingleScale::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(scale, N);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
	fprintf(f, "#scale=%d\n", scale);
    for(int i = 0; i < range; i++)
        fprintf(f, "%lf\n", Ht[i]);
    fclose(f);
}

void HTsingleScale::plot(QCustomPlot *plt)
{
    int len = getRangeLength(scale, N);
    QVector<double> plt_vec(len), w(len);
    for(int i = 0; i < len; i++){
        w[i] = i + 1;
        plt_vec[i] = Ht[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("w");
    plt->yAxis->setLabel("Ht");
    plt->graph(0)->setData(w, plt_vec);
    plt->graph(0)->setLineStyle(QCPGraph::lsLine);
    QPen pen;
    pen.setColor(Qt::red);
    plt->graph(0)->setPen(pen);
    QString fn = QString::fromStdString(file_name).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    MathOps mo;
    plt->graph(0)->setName(fn+" - <Ht> = "+QString::number(mo.mean(Ht, len)));
    plt->graph(0)->rescaleAxes();
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

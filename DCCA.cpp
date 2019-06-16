#include "DCCA.h"

DCCA::DCCA(string file_name_, string file_name2_, int min_win_, int max_win_, int ord_, string isAbs_, int win_step_)
	: FA()
{
	file_name = file_name_;
	file_name2 = file_name2_;
	min_win = min_win_;
	max_win = max_win_;
	ord = ord_;
	isAbs = isAbs_;
	win_step = win_step_;
    checkFileExistence(file_name);
    checkFileExistence(file_name2);
	getEqualLength(file_name, file_name2);
    allocateMemory();
}

DCCA::~DCCA(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<double>(y2);
	delAlloc<int>(s);
	delAlloc<double>(F);
}

void DCCA::allocateMemory(){
	t = new double [N];
	y = new double [N];
	y2 = new double [N];
	s = new int [getRangeLength(min_win, max_win, win_step)];
	F = new double [getRangeLength(min_win, max_win, win_step)];
}

void DCCA::getEqualLength(string fn1, string fn2){
	int N1 = setTsLength(fn1);
    int N2 = setTsLength(fn2);
    if(N1 > N2){
        N = N2;
    }else{
		N = N1;
	}
}

int DCCA::getTsLength(){
	return N;
}

void DCCA::setFlucVectors(){
	FA::setFlucVectors();
    MathOps mo = MathOps();
    FileOps fo = FileOps();
    double *pn2, *pn2_nomean;
    pn2 = new double [N];
    pn2_nomean = new double [N];
	FILE *f;
	f = fo.open_file(file_name2, "r");
	for(int i = 0; i < N; i++)
		fscanf(f, "%lf", &pn2[i]);
	fclose(f);
	mo.subtract_mean(pn2, N, pn2_nomean);
	mo.cumsum(pn2_nomean, y2, N);
    delAlloc<double>(pn2);
    delAlloc<double>(pn2_nomean);
}
    
bool DCCA::winFlucComp(){
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(min_win, max_win, win_step);
    ao.int_range(s, range, min_win, win_step);
	int F_len = N - min_win;
    double *F_nu;//, *t_fit, *y_fit1, *y_fit2, *diff_vec;
    F_nu = new double [F_len];
//    t_fit = new double [max_win+1];
//    y_fit1 = new double [max_win+1];
//    y_fit2 = new double [max_win+1];
//    diff_vec = new double [max_win+1];
    //computation
    //int N_s, curr_win_size;
    //int start_lim, end_lim;
    //double ang_coeff1, intercept1, ang_coeff2, intercept2;
    QProgressDialog progress(strDCCA+"\n"+QString::fromStdString(file_name.substr(file_name.find_last_of("/")+1))+
                             " vs "+QString::fromStdString(file_name2.substr(file_name2.find_last_of("/")+1)), "Stop", 0, range);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setFixedSize(xPG, yPG);
    for(int i = 0; i < range; i++){
        progress.setValue(i);
        if(progress.wasCanceled()){
            execStop = true;
            break;
        }
        int curr_win_size = s[i];
        int N_s = N - curr_win_size;
        ao.zero_vec(F_nu, F_len);
        #pragma omp parallel for
        for(int v = 0; v < N_s; v++){
            //ao.zero_vec(t_fit, max_win);
            //ao.zero_vec(y_fit1, max_win);
            //ao.zero_vec(y_fit2, max_win);
            //ao.zero_vec(diff_vec, max_win);
            int start_lim = v;
            int end_lim = v + curr_win_size;
            double *t_fit, *y_fit1, *y_fit2, *diff_vec;
            t_fit = new double [curr_win_size+1];
            y_fit1 = new double [curr_win_size+1];
            y_fit2 = new double [curr_win_size+1];
            diff_vec = new double [curr_win_size+1];
            ao.slice_vec(t, t_fit, start_lim, end_lim);
            ao.slice_vec(y, y_fit1, start_lim, end_lim);
            ao.slice_vec(y2, y_fit2, start_lim, end_lim);
            double ang_coeff1, intercept1, ang_coeff2, intercept2;
            mo.lin_fit(curr_win_size+1, t_fit, y_fit1, &ang_coeff1, &intercept1);
            mo.lin_fit(curr_win_size+1, t_fit, y_fit2, &ang_coeff2, &intercept2);
            if(isAbs.compare(CORR_DCCA) == 0){
                for(int j = 0; j <= curr_win_size; j++)
                    diff_vec[j] = (y_fit1[j] - (intercept1 + ang_coeff1 * t_fit[j])) * (y_fit2[j] - (intercept2 + ang_coeff2 * t_fit[j]));
            }else if(isAbs.compare(DEFAULT_DCCA) == 0){
                for(int j = 0; j <= curr_win_size; j++)
                    diff_vec[j] = fabs((y_fit1[j] - (intercept1 + ang_coeff1 * t_fit[j])) * (y_fit2[j] - (intercept2 + ang_coeff2 * t_fit[j])));
            }
            F_nu[v] = mo.custom_mean(diff_vec, curr_win_size+1, curr_win_size-1);
            delAlloc<double>(t_fit);
            delAlloc<double>(y_fit1);
            delAlloc<double>(y_fit2);
            delAlloc<double>(diff_vec);
        }
        if(isAbs.compare(CORR_DCCA) == 0){
            F[i] = mo.mean(F_nu, N_s);
        }else if(isAbs.compare(DEFAULT_DCCA) == 0){
            F[i] = sqrt(mo.mean(F_nu, N_s));
        }
    }
    progress.setValue(range);
    delAlloc<double>(F_nu);
//    delAlloc<double>(t_fit);
//    delAlloc<double>(y_fit1);
//    delAlloc<double>(y_fit2);
//    delAlloc<double>(diff_vec);
    return execStop;
}

double* DCCA::getF(){
    return F;
}

double DCCA::getH(){
	return H;
}

double DCCA::getH_intercept(){
	return H_intercept;
}

void DCCA::H_loglogFit(int start, int end){
    MathOps mo = MathOps();
	int range = getRangeLength(start, end, win_step);
    double *log_s, *log_F;
    log_s = new double [range];
    log_F = new double [range];
    int idx = 0;
    for(int i = (start-min_win)/win_step; i <= (end-min_win)/win_step; i++){
        log_s[idx] = log(s[i]);
        log_F[idx] = log(F[i]);
        idx++;
    }
    mo.lin_fit(range, log_s, log_F, &H, &H_intercept);
    delAlloc<double>(log_s);
    delAlloc<double>(log_F);
}

string DCCA::outFileStr(){
    size_t a = file_name.find_last_of("/");
    size_t b = file_name.find_last_of(".");
    return "/"+DCCA_FN_START+"_"+to_string(min_win)+"_"+to_string(max_win)+"_"
            +file_name.substr(a+1, b-a-1)+"_"
			+file_name2.substr(file_name2.find_last_of("/")+1);
}

void DCCA::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(min_win, max_win, win_step);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

void DCCA::plot(QCustomPlot *plt){
    int len = getRangeLength(min_win, max_win, win_step);
    QVector<double> plt_vec(len), n(len), Hfit(len);
    for(int i = 0; i < len; i++){
        n[i] = log(s[i]);
        plt_vec[i] = log(F[i]);
        Hfit[i] = H_intercept + H * n[i];
    }
    plt->addGraph();
    plt->xAxis->setLabel("log[n]");
    plt->yAxis->setLabel("log[F(n)]");
    plt->graph(0)->setData(n, plt_vec);
    plt->graph(0)->setLineStyle(QCPGraph::lsNone);
    plt->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::red, 10));
    QString fn = QString::fromStdString(file_name).split("/").last();
    fn.truncate(fn.lastIndexOf("."));
    QString fn2 = QString::fromStdString(file_name2).split("/").last();
    fn2.truncate(fn2.lastIndexOf("."));
    plt->graph(0)->setName(fn+"_"+fn2+"_"+QString::number(min_win)+"_"+QString::number(max_win));
    plt->graph(0)->rescaleAxes();
    plt->addGraph();
    plt->graph(1)->setData(n, Hfit);
    QPen pen;
    pen.setWidth(2);
    plt->graph(1)->setPen(pen);
    plt->graph(1)->setName("H = "+QString::number(H));
    plt->graph(1)->rescaleAxes(true);
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
}

#include "DFA.h"

DFA::DFA(string file_name_, int min_win_, int max_win_, int ord_, int win_step_, int rev_seg_)
	: FA()
{
	file_name = file_name_;
	min_win = min_win_;
	max_win = max_win_;
	ord = ord_;
	rev_seg = rev_seg_;
	win_step = win_step_;
    checkFileExistence(file_name);
	N = setTsLength(file_name);
    allocateMemory();
}

DFA::~DFA(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<int>(s);
	delAlloc<double>(F);
}

void DFA::allocateMemory(){
	t = new double [N];
	y = new double [N];
	s = new int [getRangeLength(min_win, max_win, win_step)];
	F = new double [getRangeLength(min_win, max_win, win_step)];
}

int DFA::getTsLength(){
	return N;
}
    
bool DFA::winFlucComp(){
    bool execStop = false;
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(min_win, max_win, win_step);
    ao.int_range(s, range, min_win, win_step);
	int F_len = N / min_win;
    double *F_nu1, *F_nu2, *t_fit, *y_fit, *diff_vec;
    F_nu1 = new double [F_len];
    F_nu2 = new double [F_len];
    t_fit = new double [max_win];
    y_fit = new double [max_win];
    diff_vec = new double [max_win];
    //computation
    int N_s, curr_win_size;
    int start_lim, end_lim;
    double ang_coeff, intercept;
    QProgressDialog progress(strDFA+"\n"+QString::fromStdString(file_name.substr(file_name.find_last_of("/")+1)), "Stop", 0, range);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setFixedSize(xPG, yPG);
    for(int i = 0; i < range; i++){
        progress.setValue(i);
        if(progress.wasCanceled()){
            execStop = true;
            break;
        }
        curr_win_size = s[i];
        N_s = N / curr_win_size;
        ao.zero_vec(F_nu1, F_len);
        for(int v = 0; v < N_s; v++){
            ao.zero_vec(t_fit, max_win);
            ao.zero_vec(y_fit, max_win);
            ao.zero_vec(diff_vec, max_win);
            start_lim = v * curr_win_size;
            end_lim = (v + 1) * curr_win_size - 1;
            ao.slice_vec(t, t_fit, start_lim, end_lim);
            ao.slice_vec(y, y_fit, start_lim, end_lim);
            mo.lin_fit(curr_win_size, t_fit, y_fit, &ang_coeff, &intercept);
            for(int j = 0; j < curr_win_size; j++)
                diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
            F_nu1[v] = mo.mean(diff_vec, curr_win_size);
        }
        if(rev_seg == 1){
            ao.zero_vec(F_nu2, F_len);
            for(int v = 0; v < N_s; v++){
                ao.zero_vec(t_fit, max_win);
                ao.zero_vec(y_fit, max_win);
                ao.zero_vec(diff_vec, max_win);
                start_lim = v * curr_win_size + (N - N_s * curr_win_size);
                end_lim = (v + 1) * curr_win_size + (N - N_s * curr_win_size) - 1;
                ao.slice_vec(t, t_fit, start_lim, end_lim);
                ao.slice_vec(y, y_fit, start_lim, end_lim);
                mo.lin_fit(curr_win_size, t_fit, y_fit, &ang_coeff, &intercept);
                for(int j = 0; j < curr_win_size; j++)
                    diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
                F_nu2[v] = mo.mean(diff_vec, curr_win_size);
            }
            F[i] = sqrt((mo.mean(F_nu1, N_s) + mo.mean(F_nu2, N_s)) / 2.0);
        }else{
            F[i] = sqrt(mo.mean(F_nu1, N_s));
        }
    }
    progress.setValue(range);
    delAlloc<double>(F_nu1);
    delAlloc<double>(F_nu2);
    delAlloc<double>(t_fit);
    delAlloc<double>(y_fit);
    delAlloc<double>(diff_vec);
    return execStop;
}

int DFA::getWinStep(){
    return win_step;
}

double DFA::getH(){
	return H;
}

double DFA::getH_intercept(){
	return H_intercept;
}

void DFA::H_loglogFit(int start, int end){
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

string DFA::outFileStr(){
    return "/"+DFA_FN_START+"_"+to_string(min_win)+"_"+to_string(max_win)+"_"+file_name.substr(file_name.find_last_of("/")+1);
}

void DFA::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(min_win, max_win, win_step);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

void DFA::plot(QCustomPlot *plt){
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
    plt->graph(0)->setName(fn+"_"+QString::number(min_win)+"_"+QString::number(max_win));
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

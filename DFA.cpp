#include "DFA.h"

DFA::DFA(string file_name_, int min_win_, int max_win_, int ord_, int rev_seg_, int win_step_)
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
    checkInputs();
    allocateMemory();
}

DFA::~DFA(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<int>(s);
	delAlloc<double>(F);
}

void DFA::checkInputs(){
	//windows size
	if(max_win < min_win){
		fprintf(stdout, "ERROR %d: biggest scale must be greater than smallest scale\n", RANGE_FAILURE);
		exit(RANGE_FAILURE);
	}else if(min_win < 3){
		fprintf(stdout, "ERROR %d: smallest scale must be greater than 2\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}else if(max_win > N){
		fprintf(stdout, "ERROR %d: biggest scale must be smaller than time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}
	//polynomial order
	if(ord < 1){
		fprintf(stdout, "ERROR %d: polynomial order must be greater than 0\n", POL_FAILURE);
		exit(POL_FAILURE);
	}
	//rev_seg
	if(rev_seg != 0 && rev_seg != 1){
		fprintf(stdout, "ERROR %d: parameter for backward computation must be 0 or 1\n", REV_SEG_FAILURE);
		exit(REV_SEG_FAILURE);
	}
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
    
void DFA::winFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(min_win, max_win, win_step);
    ao.int_range(s, range, min_win, win_step);
	int F_len = N / min_win;
    double F_nu1[F_len], F_nu2[F_len];
    double t_fit[max_win], y_fit[max_win], diff_vec[max_win];
    //computation
    int N_s, curr_win_size;
    int start_lim, end_lim;
    double ang_coeff, intercept;
    for(int i = 0; i < range; i++){
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
}

double DFA::getH(){
	return H;
}

double DFA::getH_intercept(){
	return H_intercept;
}
// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
void DFA::H_loglogFit(int start, int end){
	//if start < min_win || end > max_win -> error
    MathOps mo = MathOps();
	int range = getRangeLength(start, end);
    double log_s[range], log_F[range];
    for(int i = (start-min_win)/win_step; i <= (end-min_win)/win_step; i++){
        log_s[i] = log(s[i]);
        log_F[i] = log(F[i]);
    }
    mo.lin_fit(range, log_s, log_F, &H, &H_intercept);
}

string DFA::outFileStr(){
	return "/"+DFA_FN_START+"_"+to_string(min_win)+"_"+to_string(min_win)+"_"+file_name.substr(file_name.find_last_of("/")+1);
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void DFA::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(min_win, max_win, win_step);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

/*void DFA::plot(){
	
}*/

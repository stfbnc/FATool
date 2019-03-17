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
    checkInputs();
    allocateMemory();
}

DCCA::~DCCA(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<double>(y2);
	delAlloc<int>(s);
	delAlloc<double>(F);
}

void DCCA::checkInputs(){
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
    //dcca computation type
    if(isAbs.compare(DEFAULT_DCCA) != 0 && isAbs.compare(CORR_DCCA) != 0){
        fprintf(stdout, "ERROR %d: computation type must be %s or %s\n", REV_SEG_FAILURE, DEFAULT_DCCA, CORR_DCCA);
        exit(REV_SEG_FAILURE);
    }
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
	double pn2[N], pn2_nomean[N];
	FILE *f;
	f = fo.open_file(file_name2, "r");
	for(int i = 0; i < N; i++)
		fscanf(f, "%lf", &pn2[i]);
	fclose(f);
	mo.subtract_mean(pn2, N, pn2_nomean);
	mo.cumsum(pn2_nomean, y2, N);
}
    
void DCCA::winFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(min_win, max_win, win_step);
    ao.int_range(s, range, min_win, win_step);
	int F_len = N - min_win;
    double F_nu[F_len];
    double t_fit[max_win+1], y_fit1[max_win+1], y_fit2[max_win+1], diff_vec[max_win+1];
    //computation
    int N_s, curr_win_size;
    int start_lim, end_lim;
    double ang_coeff1, intercept1, ang_coeff2, intercept2;
    for(int i = 0; i < range; i++){
        curr_win_size = s[i];
        N_s = N - curr_win_size;
        ao.zero_vec(F_nu, F_len);
        for(int v = 0; v < N_s; v++){
            ao.zero_vec(t_fit, max_win);
            ao.zero_vec(y_fit1, max_win);
            ao.zero_vec(y_fit2, max_win);
            ao.zero_vec(diff_vec, max_win);
            start_lim = v;
            end_lim = v + curr_win_size;
            ao.slice_vec(t, t_fit, start_lim, end_lim);
            ao.slice_vec(y, y_fit1, start_lim, end_lim);
            ao.slice_vec(y2, y_fit2, start_lim, end_lim);
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
        }
        if(isAbs.compare(CORR_DCCA) == 0){
            F[i] = mo.mean(F_nu, N_s);
        }else if(isAbs.compare(DEFAULT_DCCA) == 0){
            F[i] = sqrt(mo.mean(F_nu, N_s));
        }
    }
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

// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
void DCCA::H_loglogFit(int start, int end){
	//if start < min_win || end > max_win -> error
    MathOps mo = MathOps();
	int range = getRangeLength(start, end, win_step);
    double log_s[range], log_F[range];
    for(int i = (start-min_win)/win_step; i <= (end-min_win)/win_step; i++){
        log_s[i] = log(s[i]);
        log_F[i] = log(F[i]);
    }
    mo.lin_fit(range, log_s, log_F, &H, &H_intercept);
}

string DCCA::outFileStr(){
	return "/"+DCCA_FN_START+"_"+to_string(min_win)+"_"+to_string(min_win)+"_"
			+file_name.substr(file_name.find_last_of("/")+1)+"_"
			+file_name2.substr(file_name2.find_last_of("/")+1);
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void DCCA::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(min_win, max_win, win_step);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

/*void DCCA::plot(){
	
}*/

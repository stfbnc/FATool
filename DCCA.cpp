#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "DCCA.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

DCCA::DCCA(string fileName, string fileName2, int minWin, int maxWin, int polOrd, string compType)
		: file_name(fileName), file_name2(fileName2), min_win(minWin), max_win(maxWin), ord(polOrd), isAbs(compType)
{
    checkFileExistence(fileName);
    checkFileExistence(fileName2);
	getEqualLength(fileName, fileName2);
    checkInputs(minWin, maxWin, polOrd, compType);
    allocateMemory(N, getRangeLength(minWin, maxWin));
}

DCCA::~DCCA(){
	delete[] t;
	delete[] y;
    delete[] y2;
	delete[] s;
	delete[] F;
}

void DCCA::checkInputs(int mw, int Mw, int po, string type){
	//windows size
	if(Mw < mw){
		fprintf(stdout, "ERROR %d: biggest scale must be greater than smallest scale\n", RANGE_FAILURE);
		exit(RANGE_FAILURE);
	}else if(mw < 3){
		fprintf(stdout, "ERROR %d: smallest scale must be greater than 2\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}else if(Mw > N){
		fprintf(stdout, "ERROR %d: biggest scale must be smaller than time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}
	//polynomial order
	if(po < 1){
		fprintf(stdout, "ERROR %d: polynomial order must be greater than 0\n", POL_FAILURE);
		exit(POL_FAILURE);
	}
    //dcca computation type
    if(type.compare(DEFAULT_DCCA) != 0 && type.compare(CORR_DCCA) != 0){
        fprintf(stdout, "ERROR %d: computation type must be %s or %s\n", REV_SEG_FAILURE, DEFAULT_DCCA, CORR_DCCA);
        exit(REV_SEG_FAILURE);
    }
}

void DCCA::allocateMemory(int L1, int L2){
	t = new double [L1];
	y = new double [L1];
	y2 = new double [L1];
	s = new int [L2];
	F = new double [L2];
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
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
    FileOps fo = FileOps();
	//time series vectors
	double ts1[N], ts2[N];
    double ts1_nomean[N], ts2_nomean[N];
    FILE *f;
    f = fo.open_file(file_name, "r");
    for(int i = 0; i < N; i++)
        fscanf(f, "%lf", &ts1[i]);
    fclose(f);
    f = fo.open_file(file_name2, "r");
    for(int i = 0; i < N; i++)
        fscanf(f, "%lf", &ts2[i]);
    fclose(f);
	//time vector
    ao.double_range(t, N, 1.0);
    //time series minus its mean
    mo.subtract_mean(ts1, N, ts1_nomean);
    mo.subtract_mean(ts2, N, ts2_nomean);
    //cumulative sum
    mo.cumsum(ts1_nomean, y, N);
    mo.cumsum(ts2_nomean, y2, N);
}
    
void DCCA::winFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(min_win, max_win);
    ao.int_range(s, range, min_win);
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
	int range = getRangeLength(start, end);
    double log_s[range], log_F[range];
    for(int i = start - min_win; i <= end - min_win; i++){
        log_s[i] = log(s[i]);
        log_F[i] = log(F[i]);
    }
    mo.lin_fit(range, log_s, log_F, &H, &H_intercept);
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void DCCA::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(min_win, max_win);
	FILE *f;
    f = fo.open_file(path_tot, "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

/*void DCCA::plot(){
	
}*/

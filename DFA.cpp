#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "DFA.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

DFA::DFA(string fileName, int minWin, int maxWin, int polOrd, int revSeg)
		: file_name(fileName), min_win(minWin), max_win(maxWin), ord(polOrd), rev_seg(revSeg)
{
    checkFileExistence(fileName);
	N = setTsLength(fileName);
    checkInputs(minWin, maxWin, polOrd, revSeg);
    allocateMemory(N, getRangeLength(minWin, maxWin));
}

DFA::~DFA(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
}

void DFA::checkInputs(int mw, int Mw, int po, int rvsg){
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
	//rev_seg
	if(rvsg != 0 && rvsg != 1){
		fprintf(stdout, "ERROR %d: parameter for backward computation must be 0 or 1\n", REV_SEG_FAILURE);
		exit(REV_SEG_FAILURE);
	}
}

void DFA::allocateMemory(int L1, int L2){
	t = new double [L1];
	y = new double [L1];
	s = new int [L2];
	F = new double [L2];
}

int DFA::getTsLength(){
	return N;
}

void DFA::setFlucVectors(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
    FileOps fo = FileOps();
	//time series vector
	double pn[N], pn_nomean[N];
    FILE *f;
    f = fo.open_file(file_name, "r");
    for(int i = 0; i < N; i++)
        fscanf(f, "%lf", &pn[i]);
    fclose(f);
	//time vector
    ao.double_range(t, N, 1.0);
    //time series minus its mean
    mo.subtract_mean(pn, N, pn_nomean);
    //cumulative sum
    mo.cumsum(pn_nomean, y, N);
}
    
void DFA::winFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(min_win, max_win);
    ao.int_range(s, range, min_win);
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

double* DFA::getF(){
    return F;
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
    for(int i = start - min_win; i <= end - min_win; i++){
        log_s[i] = log(s[i]);
        log_F[i] = log(F[i]);
    }
    mo.lin_fit(range, log_s, log_F, &H, &H_intercept);
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void DFA::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(min_win, max_win);
	FILE *f;
    f = fo.open_file(path_tot, "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

/*void DFA::plot(){
	
}*/

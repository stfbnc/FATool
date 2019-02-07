#include <iostream>
#include <cmath>
#include <string>
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

DFA::DFA(string FileName, int MinWin, int MaxWin int PolOrd, int RevSeg)
		: FA(FileName, MinWin, MaxWin, PolOrd, RevSeg) {}

DFA::~DFA(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
}

int DFA::GetTsLength(){
	//N
	return rows_number(file_name);
}

int DFA::GetNumScales(int start, int end){
	//range
	//if ! end > start -> error
	return end - start + 1;
}

void DFA::SetFlucVectors(){
	N = GetTsLength();
	//time series vector
	double *pn;
	pn = new double [N];
    FILE *f;
    f = fopen(file_name, "r");
    for(int i = 0; i < N; i++)
        fscanf(f, "%lf", pn + i);
    fclose(f);
	//time vector
	t = new double [N];
    double_range(t, N, 1.0);
    //time series minus its mean
	double *pn_nomean;
    pn_nomean = new double [N];
	subtract_mean(pn, N, pn_nomean);
    //cumulative sum
	y = new double [N];
    cumsum(pn_nomean, y, N);
	delete[] pn;
	delete[] pn_nomean;
}
    
void DFA::WinFlucComp(){
	N = GetTsLength();
	int range = GetNumScales(min_win, max_win);
	int_range(s, range, min_win);
	s = new int [range];
	F = new double [range];
	int F_len = N / min_win;
    double *F_nu1, *F_nu2, *t_fit, *y_fit, *diff_vec;
    F_nu1 = new double [F_len];
    F_nu2 = new double [F_len];
    t_fit = new double [max_win];
    y_fit = new double [max_win];
    diff_vec = new double [max_win];
    //computation
    int N_s;
    int start_lim, end_lim;
    double ang_coeff, intercept;
    for(int i = 0; i < range; i++){
        N_s = N / s[i];
        zero_vec(F_nu1, F_len);
        for(int v = 0; v < N_s; v++){
            zero_vec(t_fit, max_win);
            zero_vec(y_fit, max_win);
            zero_vec(diff_vec, max_win);
            start_lim = v * s[i];
            end_lim = (v + 1) * s[i];
            slice_vec(t, t_fit, start_lim, end_lim);
            slice_vec(y, y_fit, start_lim, end_lim);
            lin_fit(s[i], t_fit, y_fit, &ang_coeff, &intercept);
            for(int j = 0; j < s[i]; j++)
                diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
            F_nu1[v] = mean(diff_vec, s[i]);
        }
        if(rev_seg == 1){
            zero_vec(F_nu2, F_len);
            for(int v = 0; v < N_s; v++){
                zero_vec(t_fit, max_win);
                zero_vec(y_fit, max_win);
                zero_vec(diff_vec, max_win);
                start_lim = v * s[i] + (N - N_s * s[i]);
                end_lim = (v + 1) * s[i] + (N - N_s * s[i]);
                slice_vec(t, t_fit, start_lim, end_lim);
                slice_vec(y, y_fit, start_lim, end_lim);
                lin_fit(s[i], t_fit, y_fit, &ang_coeff, &intercept);
                for(int j = 0; j < s[i]; j++)
                    diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
                F_nu2[v] = mean(diff_vec, s[i]);
            }
            F[i] = sqrt((mean(F_nu1, N_s) + mean(F_nu2, N_s)) / (double)2);
        }else{
            F[i] = sqrt(mean(F_nu1, N_s));
        }
    }
    delete[] F_nu1;
    delete[] F_nu2;
    delete[] t_fit;
    delete[] y_fit;
    delete[] diff_vec;
}
// l'interfaccia puo' clcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
double H_loglogFit(int start, int end){
	//if start < min_win || end > max_win -> error
	int range = GetNumScales(start, end);
	double H, H_intercept;
    double *log_s, *log_F;
    log_s = new double [range];
    log_F = new double [range];
    for(int i = start - min_win; i <= end - min_win; i++){
        *(log_s + i) = log(s[i]);
        *(log_F + i) = log(F[i]);
    }
    lin_fit(range, log_s, log_F, &H, &H_intercept);
	delete[] log_s;
	delete[] log_F;
	return H;
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void SaveFile(string path_tot){
	int range = GetNumScales(min_win, max_win);
    f = fopen(path_tot, "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

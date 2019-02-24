#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "DFA.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

DFA::DFA(string FileName, int MinWin, int MaxWin, int PolOrd, int RevSeg)
		: FA(FileName, MinWin, MaxWin, PolOrd, RevSeg)
{
    CheckFileExistence(FileName);
    CheckInputs(MinWin, MaxWin, PolOrd, RevSeg);
    AllocateMemory(N, GetNumScales(MinWin, MaxWin));
}

DFA::~DFA(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
}

void DFA::SetFlucVectors(){
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
    
void DFA::WinFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = GetNumScales(min_win, max_win);
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
            end_lim = (v + 1) * curr_win_size;
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
                end_lim = (v + 1) * curr_win_size + (N - N_s * curr_win_size);
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
// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
double DFA::H_loglogFit(int start, int end){
	//if start < min_win || end > max_win -> error
    MathOps mo = MathOps();
	int range = GetNumScales(start, end);
	double H, H_intercept;
    double log_s[range], log_F[range];
    for(int i = start - min_win; i <= end - min_win; i++){
        log_s[i] = log(s[i]);
        log_F[i] = log(F[i]);
    }
    mo.lin_fit(range, log_s, log_F, &H, &H_intercept);
	return H;
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void DFA::SaveFile(string path_tot){
    FileOps fo = FileOps();
	int range = GetNumScales(min_win, max_win);
	FILE *f;
    f = fo.open_file(path_tot, "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

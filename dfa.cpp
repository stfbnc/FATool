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
    CheckInputs(file_name, min_win, max_win, ord, rev_seg);
    AllocateMemory(GetTsLength(), GetNumScales(min_win, max_win));
}

DFA::~DFA(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
}

void DFA::SetFlucVectors(){
	int N = GetTsLength();
	//time series vector
	double *pn;
	pn = new double[N];
    FILE *f;
    f = FileOps().open_file(file_name, "r");
    for(int i = 0; i < N; i++)
        fscanf(f, "%lf", pn + i);
    fclose(f);
	//time vector
    ArrayOps().double_range(t, N, 1.0);
    //time series minus its mean
	double *pn_nomean;
    pn_nomean = new double[N];
	MathOps().subtract_mean(pn, N, pn_nomean);
    //cumulative sum
    MathOps().cumsum(pn_nomean, y, N);
    delete[] pn;
    delete[] pn_nomean;
}
    
void DFA::WinFlucComp(){
	int N = GetTsLength();
	int range = GetNumScales(min_win, max_win);
	ArrayOps().int_range(s, range, min_win);
	int F_len = N / min_win;
    double *F_nu1, *F_nu2, *t_fit, *y_fit, *diff_vec;
    F_nu1 = new double[F_len];
    F_nu2 = new double[F_len];
    t_fit = new double[max_win];
    y_fit = new double[max_win];
    diff_vec = new double[max_win];
    //computation
    int N_s, curr_win_size;
    int start_lim, end_lim;
    double ang_coeff, intercept;
    for(int i = 0; i < range; i++){
        curr_win_size = *(s + i);
        N_s = N / curr_win_size;
        ArrayOps().zero_vec(F_nu1, F_len);
        for(int v = 0; v < N_s; v++){
            ArrayOps().zero_vec(t_fit, max_win);
            ArrayOps().zero_vec(y_fit, max_win);
            ArrayOps().zero_vec(diff_vec, max_win);
            start_lim = v * curr_win_size;
            end_lim = (v + 1) * curr_win_size;
            ArrayOps().slice_vec(t, t_fit, start_lim, end_lim);
            ArrayOps().slice_vec(y, y_fit, start_lim, end_lim);
            MathOps().lin_fit(curr_win_size, t_fit, y_fit, &ang_coeff, &intercept);
            for(int j = 0; j < curr_win_size; j++)
                *(diff_vec + j) = pow((*(y_fit + j) - (intercept + ang_coeff * *(t_fit + j))), 2.0);
            *(F_nu1 + v) = MathOps().mean(diff_vec, curr_win_size);
        }
        if(rev_seg == 1){
            ArrayOps().zero_vec(F_nu2, F_len);
            for(int v = 0; v < N_s; v++){
                ArrayOps().zero_vec(t_fit, max_win);
                ArrayOps().zero_vec(y_fit, max_win);
                ArrayOps().zero_vec(diff_vec, max_win);
                start_lim = v * curr_win_size + (N - N_s * curr_win_size);
                end_lim = (v + 1) * curr_win_size + (N - N_s * curr_win_size);
                ArrayOps().slice_vec(t, t_fit, start_lim, end_lim);
                ArrayOps().slice_vec(y, y_fit, start_lim, end_lim);
                MathOps().lin_fit(curr_win_size, t_fit, y_fit, &ang_coeff, &intercept);
                for(int j = 0; j < curr_win_size; j++)
                    *(diff_vec + j) = pow((*(y_fit + j) - (intercept + ang_coeff * *(t_fit + j))), 2.0);
                *(F_nu2 + v) = MathOps().mean(diff_vec, curr_win_size);
            }
            *(F + i) = sqrt((MathOps().mean(F_nu1, N_s) + MathOps().mean(F_nu2, N_s)) / 2.0);
        }else{
            *(F + i) = sqrt(MathOps().mean(F_nu1, N_s));
        }
    }
    delete[] F_nu1;
    delete[] F_nu2;
    delete[] t_fit;
    delete[] y_fit;
    delete[] diff_vec;
}
// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
double DFA::H_loglogFit(int start, int end){
	//if start < min_win || end > max_win -> error
	int range = GetNumScales(start, end);
	double H, H_intercept;
    double *log_s, *log_F;
    log_s = new double[range];
    log_F = new double[range];
    for(int i = start - min_win; i <= end - min_win; i++){
        *(log_s + i) = log(*(s + i));
        *(log_F + i) = log(*(F + i));
    }
    MathOps().lin_fit(range, log_s, log_F, &H, &H_intercept);
	delete[] log_s;
	delete[] log_F;
	return H;
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void DFA::SaveFile(string path_tot){
	int range = GetNumScales(min_win, max_win);
	FILE *f;
    f = FileOps().open_file(path_tot, "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", *(s + i), *(F + i));
    fclose(f);
}

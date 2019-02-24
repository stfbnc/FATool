#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "DCCA.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

DCCA::DCCA(string FileName, string FileName2, int MinWin, int MaxWin, int PolOrd)
		: FA(FileName, MinWin, MaxWin, PolOrd), file_name2(FileName2)
{
    CheckFileExistence(FileName);
    CheckFileExistence(FileName2);
    CheckInputs(MinWin, MaxWin, PolOrd);
    GetEqualLength(N, FileName2);
    AllocateMemory(N, GetNumScales(MinWin, MaxWin));
    y2 = new double [N];
}

DCCA::~DCCA(){
	delete[] t;
	delete[] y;
    delete[] y2;
	delete[] s;
	delete[] F;
}

void DCCA::GetEqualLength(int len, string fn2){
    int N2 = GetTsLength(fn2);
    if(N > N2){
        N = N2;
    }
}

void DCCA::SetFlucVectors(){
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
    
void DCCA::WinFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = GetNumScales(min_win, max_win);
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
            for(int j = 0; j <= curr_win_size; j++)
                diff_vec[j] = (y_fit1[j] - (intercept1 + ang_coeff1 * t_fit[j])) * (y_fit2[j] - (intercept2 + ang_coeff2 * t_fit[j]));
            F_nu[v] = mo.custom_mean(diff_vec, curr_win_size+1, curr_win_size-1);
        }
        F[i] = sqrt(mo.custom_mean(F_nu, N_s, N_s));
    }
}
// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
double DCCA::H_loglogFit(int start, int end){
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
void DCCA::SaveFile(string path_tot){
    FileOps fo = FileOps();
	int range = GetNumScales(min_win, max_win);
	FILE *f;
    f = fo.open_file(path_tot, "w");
    for(int i = 0; i < range; i++)
        fprintf(f, "%d %lf\n", s[i], F[i]);
    fclose(f);
}

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "HT.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

HT::HT(string FileName, int Scale, int PolOrd)
		: file_name(FileName), scale(Scale), ord(PolOrd)
{
    CheckFileExistence(FileName);
    //CheckInputs(MinWin, MaxWin, PolOrd, RevSeg);
    AllocateMemory(N, GetNumScales(Scale, N));
}

HT::~HT(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
}

void HT::SetFlucVectors(){
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
    
void HT::WinFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = GetNumScales(scale, N);
    double t_fit[scale], y_fit[scale], diff_vec[scale];
    //computation
    int start_lim, end_lim;
    double ang_coeff, intercept;
	ao.zero_vec(t_fit, scale);
	ao.zero_vec(y_fit, scale);
	ao.zero_vec(diff_vec, scale);
	ao.zero_vec(s, range);
	for(int v = 0; v <= N - scale; v++){
		start_lim = v;
		end_lim = v + scale - 1;
		ao.slice_vec(t, t_fit, start_lim, end_lim);
		ao.slice_vec(y, y_fit, start_lim, end_lim);
		mo.lin_fit(scale, t_fit, y_fit, &ang_coeff, &intercept);
		for(int j = 0; j < scale; j++)
			diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
		s[v] = sqrt(mo.mean(diff_vec, scale));
	}
}
// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
void HT::H_loglogFit(int start, int end){
	//tutta la parte della DFA a q = 0 va qui
	//DFA0
	//getH
	//getH_intercept
	
    MathOps mo = MathOps();
	int range = GetNumScales(scale, N);
	double Regfit, logscale;
	Regfit = Hq0_intercept + Hq0 * log(scale);
    logscale = log(range) - log(scale);
    for(int i = start; i <= end; i++){
        F[i] = (Regfit - log(s[i]))/(double) logscale + Hq0;
    }
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void HT::SaveFile(string path_tot){
    FileOps fo = FileOps();
	int range = GetNumScales(scale, N);
	FILE *f;
    f = fo.open_file(path_tot, "w");
	fprintf(f, "#scale=%d\n", scale);
    for(int i = 0; i < range; i++)
        fprintf(f, "%lf\n", F[i]);
    fclose(f);
}

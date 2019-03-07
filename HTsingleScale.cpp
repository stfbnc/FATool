#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "HTsingleScale.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"
#include "MFDFAsingleQ.h"

using namespace std;

HTsingleScale::HTsingleScale(string fileName, int inputScale, int polOrd)
		: file_name(fileName), scale(inputScale), ord(polOrd)
{
    checkFileExistence(fileName);
    checkInputs(inputScale, polOrd);
    allocateMemory(N, getRangeLength(inputScale, N));
}

HTsingleScale::~HTsingleScale(){
	delete[] t;
	delete[] y;
	delete[] F;
	delete[] Ht;
}

void HTsingleScale::checkInputs(int iScale, int po){
	//windows size
	if(iScale < 3){
		fprintf(stdout, "ERROR %d: scale must be greater than 2\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}else if(iScale > N){
		fprintf(stdout, "ERROR %d: scale must be smaller than time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}
	//polynomial order
	if(po < 1){
		fprintf(stdout, "ERROR %d: polynomial order must be greater than 0\n", POL_FAILURE);
		exit(POL_FAILURE);
	}
}

void HTsingleScale::allocateMemory(int L1, int L2){
	t = new double [L1];
	y = new double [L1];
	F = new double [L2];
	Ht = new double [L2];
}

int HTsingleScale::getTsLength(){
	return N;
}

void HTsingleScale::setFlucVectors(){
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
    
void HTsingleScale::winFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(scale, N);
    double t_fit[scale], y_fit[scale], diff_vec[scale];
    //computation
    int start_lim, end_lim;
    double ang_coeff, intercept;
	ao.zero_vec(t_fit, scale);
	ao.zero_vec(y_fit, scale);
	ao.zero_vec(diff_vec, scale);
	ao.zero_vec(F, range);
	for(int v = 0; v <= N-scale; v++){
		start_lim = v;
		end_lim = v + scale - 1;
		ao.slice_vec(t, t_fit, start_lim, end_lim);
		ao.slice_vec(y, y_fit, start_lim, end_lim);
		mo.lin_fit(scale, t_fit, y_fit, &ang_coeff, &intercept);
		for(int j = 0; j < scale; j++)
			diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
		F[v] = sqrt(mo.mean(diff_vec, scale));
	}
}
// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
void HTsingleScale::H_loglogFit(int start, int end){
	//tutta la parte della DFA a q = 0 va qui
	MFDFAsingleQ dfaQ0 = MFDFAsingleQ(file_name, 4, getTsLength()/5, 1, 0.0);
	dfaQ0.setFlucVectors();
	dfaQ0.winFlucComp();
	dfaQ0.H_loglogFit(4, getTsLength()/5);
	double Hq0 = dfaQ0.getH();
	double Hq0_intercept = dfaQ0.getH_intercept();
	
    MathOps mo = MathOps();
	int range = getRangeLength(scale, N);
	double Regfit, logscale;
	Regfit = Hq0_intercept + Hq0 * log(scale);
    logscale = log(range) - log(scale);
    for(int i = start; i <= end; i++){
        Ht[i] = (Regfit - log(F[i]))/(double) logscale + Hq0;
    }
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void HTsingleScale::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(scale, N);
	FILE *f;
    f = fo.open_file(path_tot, "w");
	fprintf(f, "#scale=%d\n", scale);
    for(int i = 0; i < range; i++)
        fprintf(f, "%lf\n", Ht[i]);
    fclose(f);
}

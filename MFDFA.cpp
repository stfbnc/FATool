#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "MFDFA.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

MFDFA::MFDFA(string fileName, int minWin, int maxWin, int polOrd, double qIn, int qLen, double qStep, int revSeg)
		: MFDFAsingleQ(fileName, minWin, maxWin, polOrd, qIn, revSeg), startq(qIn), Nq(qLen), stepq(qStep)
{
    allocateQmemory(qLen);
}

MFDFA::~MFDFA(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
    delete[] qRange;
    delete[] Hq;
    delete[] H_interceptq;
}

void MFDFA::allocateQmemory(int L){
    qRange = new double [L];
    Hq = new double [L];
    H_interceptq = new double [L];
}

void MFDFA::getQrange(double start, int len, double step){
    ArrayOps ao = ArrayOps();
    ao.double_range(qRange, len, start, step);
}

void MFDFA::qWinFlucComp(){
	getQrange(startq, Nq, stepq);
	int Lq = getRangeLength(min_win, max_win);
	double flucMtx[Lq][Nq];
	for(int i = 0; i < Nq; i++){
		q = getQrange[i];
		winFlucComp();
		for(int j = 0; j < Lq; j++){
			flucMtx[j][i] = F[j];
		}
		H_loglogFit(min_win, max_win);
		Hq[i] = getH();
		H_interceptq[i] = getH_intercept();
	}
}

void MFDFA::saveFile(string path_tot){
	FileOps fo = FileOps();
	int Lq = getRangeLength(min_win, max_win);
	FILE *f;
    f = fo.open_file(path_tot, "w");
	fprintf(f, "#q ");
	for(int i = 0; i < Lq; i++){
		i == Lq-1 ? fprintf(f, "%lf\n", Hq[i]) : fprintf(f, "%lf ", Hq[i]);
	}
    for(int i = 0; i < Lq; i++){
		fprintf(f, "%d ", s[i]);
		for(int j = 0; j < Nq; j++){
			j == Nq-1 ? fprintf(f, "%lf\n", F[i]) : fprintf(f, "%lf ", F[i]);
		}
	}
    fclose(f);
}

void MFDFA::qsaveFile(string path_tot){
	FileOps fo = FileOps();
	FILE *f;
    f = fo.open_file(path_tot, "w");
    for(int i = 0; i < Nq; i++){
		fprintf(f, "%lf %lf %lf\n", qRange[i], Hq[i], H_interceptq[i]);
	}
    fclose(f);
}

/*void MFDFA::plot(){
	
}*/

/*void MFDFA::qplot(){
	
}*/
